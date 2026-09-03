#include <iostream>
#include <complex>
#include <cmath>

int main() {
    using std::cout; using std::endl;
    std::complex<double> z(3.0, 4.0); // 3 + 4i

    cout << "z = " << z << endl;
    cout << "real(z) = " << std::real(z) << ", imag(z) = " << std::imag(z) << endl;
    cout << "abs(z) = " << std::abs(z) << " (should be 5)" << endl;
    cout << "arg(z) = " << std::arg(z) << " (radians)" << endl;
    cout << "norm(z) = " << std::norm(z) << " (square of abs)" << endl;
    cout << "conj(z) = " << std::conj(z) << endl;

    // 构造极坐标复数
    auto p = std::polar(2.0, M_PI/4.0); // r=2, theta=45deg
    cout << "polar(2, pi/4) = " << p << endl;

    // 复数幂与指数
    std::complex<double> w = std::pow(z, 2.0);
    cout << "z^2 = " << w << endl;
    cout << "exp(z) = " << std::exp(z) << endl;

    return 0;
}
