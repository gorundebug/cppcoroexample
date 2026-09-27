import concurrent.futures
import json
import urllib.request

for service, port in [('orderservice', 9091), ('inventoryservice', 9092), ('analyticsservice', 9093)]:
    for path in ['/health/startup', '/health/ready', '/health/live']:
        with urllib.request.urlopen(f'http://{service}:{port}{path}', timeout=5) as response:
            assert response.status == 200, (service, path)
    with urllib.request.urlopen(f'http://{service}:{port}/status/data', timeout=5) as response:
        graph = json.load(response)
        assert graph['nodes'] and graph['edges'], service
    print(service, 'health and graph PASS', flush=True)

def process(index):
    order_id = f'coro-concurrent-{index}'
    payload = {
        'order_id': order_id,
        'customer_id': 'acceptance-customer',
        'items': [{'order_id': order_id, 'item_id': f'item-{index}-{item}',
                   'sku': f'CORO-MISSING-{index}-{item}', 'quantity': 1, 'unit_price': 12.5}
                  for item in range(3)],
        'total_amount': 37.5,
        'created_at': '2026-09-27T00:00:00Z',
        'trace_id': order_id,
    }
    request = urllib.request.Request('http://orderservice:9091/v1/processorder',
        data=json.dumps(payload).encode(), headers={'Content-Type': 'application/json', 'X-Request-ID': order_id}, method='POST')
    with urllib.request.urlopen(request, timeout=12) as response:
        assert response.status == 200
        result = json.load(response)
    items = result['confirmed_items']
    assert len(items) == 3, result
    assert {item['item_id'] for item in items} == {item['item_id'] for item in payload['items']}, result
    assert all(item['status'] == 'OUT_OF_STOCK' and not item['reserved'] and item['available_qty'] == 0
               for item in items), result
    assert result['order_id'] == order_id, result
    return result['order_id']

with concurrent.futures.ThreadPoolExecutor(max_workers=8) as pool:
    ids = list(pool.map(process, range(32)))
assert len(set(ids)) == 32
print('32 concurrent orders / 96 gRPC item results: PASS', flush=True)
