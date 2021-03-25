// shared_ptr::swap example
#include <iostream>
#include <memory>

class test {

private:
  std::string name_;

public:
  test(const std::string name) : name_(name) {}
  ~test() {}
  void show() { std::cout << "my name is " << name_ << std::endl; }
};

int main() {
  auto A = std::make_shared<test>("a");

  std::shared_ptr<test> B = A;

  A->show();
  B->show();

  A.reset(new test("new a"));

  std::cout << "after changed " << std::endl;

  A->show();
  B->show();

  return 0;
}