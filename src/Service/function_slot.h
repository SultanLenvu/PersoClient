#ifndef FUNCTIONSLOT_H
#define FUNCTIONSLOT_H

class AsyncFunctionDecorator {
 public:
  explicit AsyncFunctionDecorator() = default;
  ~AsyncFunctionDecorator() = default;

 public:
  void operator()();
};

#endif // FUNCTIONSLOT_H
