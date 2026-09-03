// 基类模板：使用 CRTP
template<typename Derived>
class Base {
public:
    void run() {
        // 编译期调用派生类的实现
        static_cast<Derived*>(this)->runImpl();
    }
};

// 派生类：继承自 Base<自身>
class Son : public Base<Son> {
public:
    void runImpl() {
        std::cout << "Son is running!\n";
    }
};
