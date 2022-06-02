from email.mime import base
import sys
import time
import requests
import base64
from textops import *
import re
import requests_cache
from typing import List

requests_cache.install_cache("demo_cache", backend="filesystem")

# if __name__ == "__main__":
#     print(requests_cache.get_cache())
#     print("Cached URLS:")
#     print("\n".join(requests_cache.get_cache().urls))
#     sys.exit(0)

MAX_PAGES = 50
curr_page = 1

headers = {"Authorization": "Token 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"}


def get_page(page=1):
    print(page)
    url = f"https://api.github.com/search/code?q=SIMILAR TO +in:file +language:sql&per_page=1000&page={page}"

    response = requests.request("GET", url, headers=headers)
    resp = response.json()
    if resp.get("total_count") is None:
        print(resp)
        time.sleep(120)
        return get_page(page)
    else:
        print("Items: ", len(resp["items"]))
        return resp


pattern = "similar to ['\`‘\"](.*?)['\`’\"]"
# print(re.findall(pattern, 'SIMILAR to "a"', flags=re.IGNORECASE))

longest_pattern = 0
patterns = set()
while curr_page <= MAX_PAGES:
    resp = get_page(curr_page)
    curr_page += 1
    for item in resp["items"]:
        # print(item["url"])
        r2 = requests.request("GET", item["url"], headers=headers)
        cntnt = r2.json()
        if cntnt.get("content", None) is None:
            print(cntnt)
            time.sleep(10)
            continue
        else:
            cntnt = cntnt["content"]

        st = base64.b64decode(cntnt).decode("utf-8")
        xxx: List[str] = re.findall(pattern, st, flags=re.IGNORECASE)
        xxx = [x.strip() for x in xxx]
        # print("Size: ", xxx)
        if len(xxx) > 0:
            ls = max(xxx, key=len)
            longest_pattern = max(longest_pattern, len(ls))
        patterns.update(xxx)

    with open("pats_x.txt", "w") as f:
        f.write("\n".join(patterns))
        print("Curr size: ", len(patterns))
        print("Longest Pattern: ", longest_pattern)


print(len(patterns))
