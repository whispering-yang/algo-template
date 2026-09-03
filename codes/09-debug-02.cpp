#include <bits/stdc++.h>
using namespace std;

int divide(int a, int b) {
    return a / b;  // 当 b == 0 时触发浮点异常
}

vector<int> buildPrefixSum(const vector<int>& arr) {
    int n = arr.size();
    vector<int> pref(n);
    pref[0] = arr[0];
    for (int i = 1; i <= n; i++) {   // 越界错误：应为 i < n
        pref[i] = pref[i - 1] + arr[i];
    }
    return pref;
}

int main() {
    ios::sync_with_stdio(false), cin.tie(nullptr);

    vector<int> a = {1, 2, 3, 4, 5};
    auto pref = buildPrefixSum(a);

    for (int i = 0; i <= a.size(); i++) {  // 越界输出
        cout << pref[i] << " \n"[i + 1 == a.size()];
    }

    int x = 10, y = 0;
    cout << divide(x, y) << "\n";          // 除零崩溃

    return 0;
}
