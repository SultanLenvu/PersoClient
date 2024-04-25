#ifndef IASYNCCLIENT_H
#define IASYNCCLIENT_H


class IAsyncClient
{
 public:
  explicit IAsyncClient() = default;
  virtual ~IAsyncClient() = default;

 public:
  virtual void asyncStart(void) = 0;
  virtual void asyncFinish(void) = 0;
};

#endif // IASYNCCLIENT_H
