// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#include "pch.h"
#include "ReactModuleBuilderMock.h"

#include <sstream>
#include "NativeModules.h"
#include "future/futureWait.h"

namespace winrt::Microsoft::ReactNative {

struct Point2 {
  int X;
  int Y;
};

FieldMap GetStructInfo(Point2 *) noexcept {
  return {{L"X", &Point2::X}, {L"Y", &Point2::Y}};
}

struct SimpleNativeModule2 {
  void Initialize(IReactContext const &context) noexcept {
    IsInitialized = true;
    TestCheck(context != nullptr);

    // Event and Function fields are initialized before REACT_INIT method call.
    TestCheck(this->OnIntEvent != nullptr);
    TestCheck(this->JSIntFunction != nullptr);
  }

  int Add(int x, int y) noexcept {
    return x + y;
  }

  int Negate(int x) noexcept {
    return -x;
  }

  std::string SayHello() noexcept {
    return "Hello";
  }

  static int StaticAdd(int x, int y) noexcept {
    return x + y;
  }

  static int StaticNegate(int x) noexcept {
    return -x;
  }

  static std::string StaticSayHello() noexcept {
    return "Hello";
  }

  void SayHello0() noexcept {
    Message = "Hello_0";
  }

  void PrintPoint(Point2 pt) noexcept {
    std::stringstream ss;
    ss << "Point2: (" << pt.X << ", " << pt.Y << ")";
    Message = ss.str();
  }

  void PrintLine(Point2 start, Point2 end) noexcept {
    std::stringstream ss;
    ss << "Line: (" << start.X << ", " << start.Y << ")-(" << end.X << ", " << end.Y << ")";
    Message = ss.str();
  }

  static void StaticSayHello1() noexcept {
    StaticMessage = "Hello_1";
  }

  static void StaticPrintPoint(Point2 pt) noexcept {
    std::stringstream ss;
    ss << "Static Point2: (" << pt.X << ", " << pt.Y << ")";
    StaticMessage = ss.str();
  }

  static void StaticPrintLine(Point2 start, Point2 end) noexcept {
    std::stringstream ss;
    ss << "Static Line: (" << start.X << ", " << start.Y << ")-(" << end.X << ", " << end.Y << ")";
    StaticMessage = ss.str();
  }

  void AddCallback(int x, int y, std::function<void(int)> const &resolve) noexcept {
    resolve(x + y);
  }

  void NegateCallback(int x, std::function<void(int)> const &resolve) noexcept {
    resolve(-x);
  }

  fire_and_forget NegateAsyncCallback(int x, std::function<void(int)> resolve) noexcept {
    co_await winrt::resume_background();
    resolve(-x);
  }

  void NegateDispatchQueueCallback(int x, std::function<void(int)> const &resolve) noexcept {
    Mso::DispatchQueue::ConcurrentQueue().Post([ x, resolve ]() noexcept { resolve(-x); });
  }

  void NegateFutureCallback(int x, std::function<void(int)> const &resolve) noexcept {
    Mso::PostFuture([ x, resolve ]() noexcept { resolve(-x); });
  }

  void SayHelloCallback(std::function<void(const std::string &)> const &resolve) noexcept {
    resolve("Hello_2");
  }

  static void StaticAddCallback(int x, int y, std::function<void(int)> const &resolve) noexcept {
    resolve(x + y);
  }

  static void StaticNegateCallback(int x, std::function<void(int)> const &resolve) noexcept {
    resolve(-x);
  }

  static fire_and_forget StaticNegateAsyncCallback(int x, std::function<void(int)> resolve) noexcept {
    co_await winrt::resume_background();
    resolve(-x);
  }

  static void StaticNegateDispatchQueueCallback(int x, std::function<void(int)> const &resolve) noexcept {
    Mso::DispatchQueue::ConcurrentQueue().Post([ x, resolve ]() noexcept { resolve(-x); });
  }

  static void StaticNegateFutureCallback(int x, std::function<void(int)> const &resolve) noexcept {
    Mso::PostFuture([ x, resolve ]() noexcept { resolve(-x); });
  }

  static void StaticSayHelloCallback(std::function<void(const std::string &)> const &resolve) noexcept {
    resolve("Static Hello_2");
  }

  void DivideCallbacks(
      int x,
      int y,
      std::function<void(int)> const &resolve,
      std::function<void(std::string const &)> const &reject) noexcept {
    if (y != 0) {
      resolve(x / y);
    } else {
      reject("Division by 0");
    }
  }

  void NegateCallbacks(
      int x,
      std::function<void(int)> const &resolve,
      std::function<void(std::string const &)> const &reject) noexcept {
    if (x >= 0) {
      resolve(-x);
    } else {
      reject("Already negative");
    }
  }

  fire_and_forget NegateAsyncCallbacks(
      int x,
      std::function<void(int)> resolve,
      std::function<void(std::string const &)> reject) noexcept {
    co_await winrt::resume_background();
    if (x >= 0) {
      resolve(-x);
    } else {
      reject("Already negative");
    }
  }

  void NegateDispatchQueueCallbacks(
      int x,
      std::function<void(int)> const &resolve,
      std::function<void(std::string const &)> const &reject) noexcept {
    Mso::DispatchQueue::ConcurrentQueue().Post([ x, resolve, reject ]() noexcept {
      if (x >= 0) {
        resolve(-x);
      } else {
        reject("Already negative");
      }
    });
  }

  void NegateFutureCallbacks(
      int x,
      std::function<void(int)> const &resolve,
      std::function<void(std::string const &)> const &reject) noexcept {
    Mso::PostFuture([ x, resolve, reject ]() noexcept {
      if (x >= 0) {
        resolve(-x);
      } else {
        reject("Already negative");
      }
    });
  }

  void ResolveSayHelloCallbacks(
      std::function<void(std::string const &)> const &resolve,
      std::function<void(std::string const &)> const & /*reject*/) noexcept {
    resolve("Hello_3");
  }

  void RejectSayHelloCallbacks(
      std::function<void(std::string const &)> const & /*resolve*/,
      std::function<void(std::string const &)> const &reject) noexcept {
    reject("Goodbye");
  }

  static void StaticDivideCallbacks(
      int x,
      int y,
      std::function<void(int)> const &resolve,
      std::function<void(std::string const &)> const &reject) noexcept {
    if (y != 0) {
      resolve(x / y);
    } else {
      reject("Division by 0");
    }
  }

  static void StaticNegateCallbacks(
      int x,
      std::function<void(int)> const &resolve,
      std::function<void(std::string const &)> const &reject) noexcept {
    if (x >= 0) {
      resolve(-x);
    } else {
      reject("Already negative");
    }
  }

  static fire_and_forget StaticNegateAsyncCallbacks(
      int x,
      std::function<void(int)> resolve,
      std::function<void(std::string const &)> reject) noexcept {
    co_await winrt::resume_background();
    if (x >= 0) {
      resolve(-x);
    } else {
      reject("Already negative");
    }
  }

  static void StaticNegateDispatchQueueCallbacks(
      int x,
      std::function<void(int)> const &resolve,
      std::function<void(std::string const &)> const &reject) noexcept {
    Mso::DispatchQueue::ConcurrentQueue().Post([ x, resolve, reject ]() noexcept {
      if (x >= 0) {
        resolve(-x);
      } else {
        reject("Already negative");
      }
    });
  }

  static void StaticNegateFutureCallbacks(
      int x,
      std::function<void(int)> const &resolve,
      std::function<void(std::string const &)> const &reject) noexcept {
    Mso::PostFuture([ x, resolve, reject ]() noexcept {
      if (x >= 0) {
        resolve(-x);
      } else {
        reject("Already negative");
      }
    });
  }

  static void StaticResolveSayHelloCallbacks(
      std::function<void(std::string const &)> const &resolve,
      std::function<void(std::string const &)> const & /*reject*/) noexcept {
    resolve("Hello_3");
  }

  static void StaticRejectSayHelloCallbacks(
      std::function<void(std::string const &)> const & /*resolve*/,
      std::function<void(std::string const &)> const &reject) noexcept {
    reject("Goodbye");
  }

  void DividePromise(int x, int y, ReactPromise<int> const &result) noexcept {
    if (y != 0) {
      result.Resolve(x / y);
    } else {
      ReactError error{};
      error.Message = "Division by 0";
      result.Reject(std::move(error));
    }
  }

  void NegatePromise(int x, ReactPromise<int> const &result) noexcept {
    if (x >= 0) {
      result.Resolve(-x);
    } else {
      ReactError error{};
      error.Message = "Already negative";
      result.Reject(std::move(error));
    }
  }

  fire_and_forget NegateAsyncPromise(int x, ReactPromise<int> result) noexcept {
    co_await winrt::resume_background();
    if (x >= 0) {
      result.Resolve(-x);
    } else {
      ReactError error{};
      error.Message = "Already negative";
      result.Reject(std::move(error));
    }
  }

  void NegateDispatchQueuePromise(int x, ReactPromise<int> const &result) noexcept {
    Mso::DispatchQueue::ConcurrentQueue().Post([ x, result ]() noexcept {
      if (x >= 0) {
        result.Resolve(-x);
      } else {
        ReactError error{};
        error.Message = "Already negative";
        result.Reject(std::move(error));
      }
    });
  }

  void NegateFuturePromise(int x, ReactPromise<int> const &result) noexcept {
    Mso::PostFuture([ x, result ]() noexcept {
      if (x >= 0) {
        result.Resolve(-x);
      } else {
        ReactError error{};
        error.Message = "Already negative";
        result.Reject(std::move(error));
      }
    });
  }

  void VoidPromise(int x, ReactPromise<void> const &result) noexcept {
    if (x % 2 == 0) {
      result.Resolve();
    } else {
      result.Reject("Odd unexpected");
    }
  }

  void ResolveSayHelloPromise(ReactPromise<std::string> const &result) noexcept {
    result.Resolve("Hello_4");
  }

  void RejectSayHelloPromise(ReactPromise<std::string> const &result) noexcept {
    ReactError error{};
    error.Message = "Promise rejected";
    result.Reject(std::move(error));
  }

  static void StaticDividePromise(int x, int y, ReactPromise<int> const &result) noexcept {
    if (y != 0) {
      result.Resolve(x / y);
    } else {
      ReactError error{};
      error.Message = "Division by 0";
      result.Reject(std::move(error));
    }
  }

  static void StaticNegatePromise(int x, ReactPromise<int> const &result) noexcept {
    if (x >= 0) {
      result.Resolve(-x);
    } else {
      ReactError error{};
      error.Message = "Already negative";
      result.Reject(std::move(error));
    }
  }

  static fire_and_forget StaticNegateAsyncPromise(int x, ReactPromise<int> result) noexcept {
    co_await winrt::resume_background();
    if (x >= 0) {
      result.Resolve(-x);
    } else {
      ReactError error{};
      error.Message = "Already negative";
      result.Reject(std::move(error));
    }
  }

  static void StaticNegateDispatchQueuePromise(int x, ReactPromise<int> const &result) noexcept {
    Mso::DispatchQueue::ConcurrentQueue().Post([ x, result ]() noexcept {
      if (x >= 0) {
        result.Resolve(-x);
      } else {
        ReactError error{};
        error.Message = "Already negative";
        result.Reject(std::move(error));
      }
    });
  }

  static void StaticNegateFuturePromise(int x, ReactPromise<int> const &result) noexcept {
    Mso::PostFuture([ x, result ]() noexcept {
      if (x >= 0) {
        result.Resolve(-x);
      } else {
        ReactError error{};
        error.Message = "Already negative";
        result.Reject(std::move(error));
      }
    });
  }

  void StaticVoidPromise(int x, ReactPromise<void> const &result) noexcept {
    if (x % 2 == 0) {
      result.Resolve();
    } else {
      result.Reject("Odd unexpected");
    }
  }

  static void StaticResolveSayHelloPromise(ReactPromise<std::string> const &result) noexcept {
    result.Resolve("Hello_4");
  }

  static void StaticRejectSayHelloPromise(ReactPromise<std::string> const &result) noexcept {
    ReactError error{};
    error.Message = "Promise rejected";
    result.Reject(std::move(error));
  }

  int AddSync(int x, int y) noexcept {
    return x + y;
  }

  int NegateSync(int x) noexcept {
    return -x;
  }

  std::string SayHelloSync() noexcept {
    return "Hello";
  }

  static int StaticAddSync(int x, int y) noexcept {
    return x + y;
  }

  static int StaticNegateSync(int x) noexcept {
    return -x;
  }

  static std::string StaticSayHelloSync() noexcept {
    return "Hello";
  }

  const std::string Constant1{"MyConstant1"};

  const std::string Constant2{"MyConstant2"};

  static constexpr Point2 Constant3{/*X =*/2, /*Y =*/3};

  static constexpr Point2 Constant4{/*X =*/3, /*Y =*/4};

  void Constant5(ReactConstantProvider &provider) noexcept {
    provider.Add(L"const51", Point2{/*X =*/12, /*Y =*/14});
    provider.Add(L"const52", "MyConstant52");
  }

  static void Constant6(ReactConstantProvider &provider) noexcept {
    provider.Add(L"const61", Point2{/*X =*/15, /*Y =*/17});
    provider.Add(L"const62", "MyConstant62");
  }

  // Allows to emit native module events
  std::function<void(int)> OnIntEvent;

  // An event without arguments
  std::function<void()> OnNoArgEvent;

  // An event with two arguments
  std::function<void(Point2 const &, Point2 const &)> OnTwoArgsEvent;

  std::function<void(Point2 const &)> OnPointEvent;

  std::function<void(char const *)> OnStringEvent;

  std::function<void(const JSValue &)> OnJSValueEvent;

  // Allows to call JS functions.
  std::function<void(int)> JSIntFunction;

  std::function<void(Point2 const &)> JSPointFunction;

  std::function<void(Point2 const &, Point2 const &)> JSLineFunction;

  // Use no arguments.
  std::function<void()> JSNoArgFunction;

  std::function<void(char const *)> JSStringFunction;

  // Use JSValue which is an immutable JSON-like data representation.
  std::function<void(const JSValue &)> JSValueFunction;

 public: // Used to report some test messages
  bool IsInitialized{false};
  std::string Message;
  static std::string StaticMessage;
};

/*static*/ std::string SimpleNativeModule2::StaticMessage;

void RegisterModule(ReactModuleBuilder<SimpleNativeModule2> &moduleBuilder) noexcept {
  moduleBuilder.RegisterModuleName(L"SimpleNativeModule2");
  moduleBuilder.RegisterInitMethod(&SimpleNativeModule2::Initialize);
  moduleBuilder.RegisterMethod(&SimpleNativeModule2::Add, L"Add");
  moduleBuilder.RegisterMethod(&SimpleNativeModule2::Negate, L"Negate");
  moduleBuilder.RegisterMethod(&SimpleNativeModule2::SayHello, L"SayHello");
  moduleBuilder.RegisterMethod(&SimpleNativeModule2::StaticAdd, L"StaticAdd");
  moduleBuilder.RegisterMethod(&SimpleNativeModule2::StaticNegate, L"StaticNegate");
  moduleBuilder.RegisterMethod(&SimpleNativeModule2::StaticSayHello, L"StaticSayHello");
  moduleBuilder.RegisterMethod(&SimpleNativeModule2::SayHello0, L"SayHello0");
  moduleBuilder.RegisterMethod(&SimpleNativeModule2::PrintPoint, L"PrintPoint");
  moduleBuilder.RegisterMethod(&SimpleNativeModule2::PrintLine, L"PrintLine");
  moduleBuilder.RegisterMethod(&SimpleNativeModule2::StaticSayHello1, L"StaticSayHello1");
  moduleBuilder.RegisterMethod(&SimpleNativeModule2::StaticPrintPoint, L"StaticPrintPoint");
  moduleBuilder.RegisterMethod(&SimpleNativeModule2::StaticPrintLine, L"StaticPrintLine");
  moduleBuilder.RegisterMethod(&SimpleNativeModule2::AddCallback, L"AddCallback");
  moduleBuilder.RegisterMethod(&SimpleNativeModule2::NegateCallback, L"NegateCallback");
  moduleBuilder.RegisterMethod(&SimpleNativeModule2::NegateAsyncCallback, L"NegateAsyncCallback");
  moduleBuilder.RegisterMethod(&SimpleNativeModule2::NegateDispatchQueueCallback, L"NegateDispatchQueueCallback");
  moduleBuilder.RegisterMethod(&SimpleNativeModule2::NegateFutureCallback, L"NegateFutureCallback");
  moduleBuilder.RegisterMethod(&SimpleNativeModule2::SayHelloCallback, L"SayHelloCallback");
  moduleBuilder.RegisterMethod(&SimpleNativeModule2::StaticAddCallback, L"StaticAddCallback");
  moduleBuilder.RegisterMethod(&SimpleNativeModule2::StaticNegateCallback, L"StaticNegateCallback");
  moduleBuilder.RegisterMethod(&SimpleNativeModule2::StaticNegateAsyncCallback, L"StaticNegateAsyncCallback");
  moduleBuilder.RegisterMethod(
      &SimpleNativeModule2::StaticNegateDispatchQueueCallback, L"StaticNegateDispatchQueueCallback");
  moduleBuilder.RegisterMethod(&SimpleNativeModule2::StaticNegateFutureCallback, L"StaticNegateFutureCallback");
  moduleBuilder.RegisterMethod(&SimpleNativeModule2::StaticSayHelloCallback, L"StaticSayHelloCallback");
  moduleBuilder.RegisterMethod(&SimpleNativeModule2::DivideCallbacks, L"DivideCallbacks");
  moduleBuilder.RegisterMethod(&SimpleNativeModule2::NegateCallbacks, L"NegateCallbacks");
  moduleBuilder.RegisterMethod(&SimpleNativeModule2::NegateAsyncCallbacks, L"NegateAsyncCallbacks");
  moduleBuilder.RegisterMethod(&SimpleNativeModule2::NegateDispatchQueueCallbacks, L"NegateDispatchQueueCallbacks");
  moduleBuilder.RegisterMethod(&SimpleNativeModule2::NegateFutureCallbacks, L"NegateFutureCallbacks");
  moduleBuilder.RegisterMethod(&SimpleNativeModule2::ResolveSayHelloCallbacks, L"ResolveSayHelloCallbacks");
  moduleBuilder.RegisterMethod(&SimpleNativeModule2::RejectSayHelloCallbacks, L"RejectSayHelloCallbacks");
  moduleBuilder.RegisterMethod(&SimpleNativeModule2::StaticDivideCallbacks, L"StaticDivideCallbacks");
  moduleBuilder.RegisterMethod(&SimpleNativeModule2::StaticNegateCallbacks, L"StaticNegateCallbacks");
  moduleBuilder.RegisterMethod(&SimpleNativeModule2::StaticNegateAsyncCallbacks, L"StaticNegateAsyncCallbacks");
  moduleBuilder.RegisterMethod(
      &SimpleNativeModule2::StaticNegateDispatchQueueCallbacks, L"StaticNegateDispatchQueueCallbacks");
  moduleBuilder.RegisterMethod(&SimpleNativeModule2::StaticNegateFutureCallbacks, L"StaticNegateFutureCallbacks");
  moduleBuilder.RegisterMethod(&SimpleNativeModule2::StaticResolveSayHelloCallbacks, L"StaticResolveSayHelloCallbacks");
  moduleBuilder.RegisterMethod(&SimpleNativeModule2::StaticRejectSayHelloCallbacks, L"StaticRejectSayHelloCallbacks");
  moduleBuilder.RegisterMethod(&SimpleNativeModule2::DividePromise, L"DividePromise");
  moduleBuilder.RegisterMethod(&SimpleNativeModule2::NegatePromise, L"NegatePromise");
  moduleBuilder.RegisterMethod(&SimpleNativeModule2::NegateAsyncPromise, L"NegateAsyncPromise");
  moduleBuilder.RegisterMethod(&SimpleNativeModule2::NegateDispatchQueuePromise, L"NegateDispatchQueuePromise");
  moduleBuilder.RegisterMethod(&SimpleNativeModule2::NegateFuturePromise, L"NegateFuturePromise");
  moduleBuilder.RegisterMethod(&SimpleNativeModule2::VoidPromise, L"voidPromise");
  moduleBuilder.RegisterMethod(&SimpleNativeModule2::ResolveSayHelloPromise, L"ResolveSayHelloPromise");
  moduleBuilder.RegisterMethod(&SimpleNativeModule2::RejectSayHelloPromise, L"RejectSayHelloPromise");
  moduleBuilder.RegisterMethod(&SimpleNativeModule2::StaticDividePromise, L"StaticDividePromise");
  moduleBuilder.RegisterMethod(&SimpleNativeModule2::StaticNegatePromise, L"StaticNegatePromise");
  moduleBuilder.RegisterMethod(&SimpleNativeModule2::StaticNegateAsyncPromise, L"StaticNegateAsyncPromise");
  moduleBuilder.RegisterMethod(
      &SimpleNativeModule2::StaticNegateDispatchQueuePromise, L"StaticNegateDispatchQueuePromise");
  moduleBuilder.RegisterMethod(&SimpleNativeModule2::StaticNegateFuturePromise, L"StaticNegateFuturePromise");
  moduleBuilder.RegisterMethod(&SimpleNativeModule2::StaticVoidPromise, L"staticVoidPromise");
  moduleBuilder.RegisterMethod(&SimpleNativeModule2::StaticResolveSayHelloPromise, L"StaticResolveSayHelloPromise");
  moduleBuilder.RegisterMethod(&SimpleNativeModule2::StaticRejectSayHelloPromise, L"StaticRejectSayHelloPromise");
  moduleBuilder.RegisterSyncMethod(&SimpleNativeModule2::AddSync, L"AddSync");
  moduleBuilder.RegisterSyncMethod(&SimpleNativeModule2::NegateSync, L"NegateSync");
  moduleBuilder.RegisterSyncMethod(&SimpleNativeModule2::SayHelloSync, L"SayHelloSync");
  moduleBuilder.RegisterSyncMethod(&SimpleNativeModule2::StaticAddSync, L"StaticAddSync");
  moduleBuilder.RegisterSyncMethod(&SimpleNativeModule2::StaticNegateSync, L"StaticNegateSync");
  moduleBuilder.RegisterSyncMethod(&SimpleNativeModule2::StaticSayHelloSync, L"StaticSayHelloSync");
  moduleBuilder.RegisterConstantField(&SimpleNativeModule2::Constant1, L"Constant1");
  moduleBuilder.RegisterConstantField(&SimpleNativeModule2::Constant2, L"const2");
  moduleBuilder.RegisterConstantField(&SimpleNativeModule2::Constant3, L"const3");
  moduleBuilder.RegisterConstantField(&SimpleNativeModule2::Constant4, L"Constant4");
  moduleBuilder.RegisterConstantMethod(&SimpleNativeModule2::Constant5, L"Constant5");
  moduleBuilder.RegisterConstantMethod(&SimpleNativeModule2::Constant6, L"Constant6");
  moduleBuilder.RegisterEventField(&SimpleNativeModule2::OnIntEvent, L"OnIntEvent");
  moduleBuilder.RegisterEventField(&SimpleNativeModule2::OnNoArgEvent, L"OnNoArgEvent");
  moduleBuilder.RegisterEventField(&SimpleNativeModule2::OnTwoArgsEvent, L"OnTwoArgsEvent");
  moduleBuilder.RegisterEventField(&SimpleNativeModule2::OnPointEvent, L"onPointEvent");
  moduleBuilder.RegisterEventField(&SimpleNativeModule2::OnStringEvent, L"onStringEvent", L"MyEventEmitter");
  moduleBuilder.RegisterEventField(&SimpleNativeModule2::OnJSValueEvent, L"OnJSValueEvent");
  moduleBuilder.RegisterFunctionField(&SimpleNativeModule2::JSIntFunction, L"JSIntFunction");
  moduleBuilder.RegisterFunctionField(&SimpleNativeModule2::JSPointFunction, L"pointFunc");
  moduleBuilder.RegisterFunctionField(&SimpleNativeModule2::JSLineFunction, L"lineFunc");
  moduleBuilder.RegisterFunctionField(&SimpleNativeModule2::JSNoArgFunction, L"JSNoArgFunction");
  moduleBuilder.RegisterFunctionField(&SimpleNativeModule2::JSStringFunction, L"stringFunc", L"MyModule");
  moduleBuilder.RegisterFunctionField(&SimpleNativeModule2::JSValueFunction, L"JSValueFunction");
}

TEST_CLASS (NoAttributeNativeModuleTest) {
  ReactModuleBuilderMock m_builderMock{};
  IReactModuleBuilder m_moduleBuilder;
  Windows::Foundation::IInspectable m_moduleObject{nullptr};
  SimpleNativeModule2 *m_module;

  NoAttributeNativeModuleTest() {
    m_moduleBuilder = make<ReactModuleBuilderImpl>(m_builderMock);
    auto provider = MakeModuleProvider<SimpleNativeModule2>();
    m_moduleObject = m_builderMock.CreateModule(provider, m_moduleBuilder);
    auto reactModule = m_moduleObject.as<IBoxedValue>();
    m_module = &BoxedValue<SimpleNativeModule2>::GetImpl(reactModule);
  }

  TEST_METHOD(TestMethodCall_Add) {
    m_builderMock.Call1(L"Add", std::function<void(int)>([](int result) noexcept { TestCheck(result == 8); }), 3, 5);
    TestCheck(m_builderMock.IsResolveCallbackCalled());
  }

  TEST_METHOD(TestMethodCall_Negate) {
    m_builderMock.Call1(L"Negate", std::function<void(int)>([](int result) noexcept { TestCheck(result == -3); }), 3);
    TestCheck(m_builderMock.IsResolveCallbackCalled());
  }

  TEST_METHOD(TestMethodCall_SayHello) {
    m_builderMock.Call1(L"SayHello", std::function<void(const std::string &)>([](const std::string &result) noexcept {
                          TestCheck(result == "Hello");
                        }));
    TestCheck(m_builderMock.IsResolveCallbackCalled());
  }

  TEST_METHOD(TestMethodCall_StaticAdd) {
    m_builderMock.Call1(
        L"StaticAdd", std::function<void(int)>([](int result) noexcept { TestCheck(result == 25); }), 20, 5);
    TestCheck(m_builderMock.IsResolveCallbackCalled());
  }

  TEST_METHOD(TestMethodCall_StaticNegate) {
    m_builderMock.Call1(
        L"StaticNegate", std::function<void(int)>([](int result) noexcept { TestCheck(result == -7); }), 7);
    TestCheck(m_builderMock.IsResolveCallbackCalled());
  }

  TEST_METHOD(TestMethodCall_StaticSayHello) {
    m_builderMock.Call1(L"StaticSayHello", std::function<void(const std::string &)>([
                        ](const std::string &result) noexcept { TestCheck(result == "Hello"); }));
    TestCheck(m_builderMock.IsResolveCallbackCalled());
    TestCheck(m_builderMock.IsResolveCallbackCalled());
  }

  TEST_METHOD(TestMethodCall_SayHello0) {
    m_builderMock.Call0(L"SayHello0");
    TestCheck(m_module->Message == "Hello_0");
  }

  TEST_METHOD(TestMethodCall_PrintPoint) {
    m_builderMock.Call0(L"PrintPoint", Point2{/*X =*/3, /*Y =*/5});
    TestCheck(m_module->Message == "Point2: (3, 5)");
  }

  TEST_METHOD(TestMethodCall_PrintLine) {
    m_builderMock.Call0(L"PrintLine", Point2{/*X =*/3, /*Y =*/5}, Point2{/*X =*/6, /*Y =*/8});
    TestCheck(m_module->Message == "Line: (3, 5)-(6, 8)");
  }

  TEST_METHOD(TestMethodCall_StaticSayHello1) {
    m_builderMock.Call0(L"StaticSayHello1");
    TestCheck(SimpleNativeModule2::StaticMessage == "Hello_1");
  }

  TEST_METHOD(TestMethodCall_StaticPrintPoint) {
    m_builderMock.Call0(L"StaticPrintPoint", Point2{/*X =*/13, /*Y =*/15});
    TestCheck(SimpleNativeModule2::StaticMessage == "Static Point2: (13, 15)");
  }

  TEST_METHOD(TestMethodCall_StaticPrintLine) {
    m_builderMock.Call0(L"StaticPrintLine", Point2{/*X =*/13, /*Y =*/15}, Point2{/*X =*/16, /*Y =*/18});
    TestCheck(SimpleNativeModule2::StaticMessage == "Static Line: (13, 15)-(16, 18)");
  }

  TEST_METHOD(TestMethodCall_AddCallback) {
    m_builderMock.Call1(
        L"AddCallback", std::function<void(int)>([](int result) noexcept { TestCheck(result == -1); }), 7, -8);
    TestCheck(m_builderMock.IsResolveCallbackCalled());
  }

  TEST_METHOD(TestMethodCall_NegateCallback) {
    m_builderMock.Call1(
        L"NegateCallback", std::function<void(int)>([](int result) noexcept { TestCheck(result == -4); }), 4);
    TestCheck(m_builderMock.IsResolveCallbackCalled());
  }

  TEST_METHOD(TestMethodCall_NegateAsyncCallback) {
    Mso::FutureWait(m_builderMock.Call1(
        L"NegateAsyncCallback", std::function<void(int)>([](int result) noexcept { TestCheck(result == -4); }), 4));
    TestCheck(m_builderMock.IsResolveCallbackCalled());
  }

  TEST_METHOD(TestMethodCall_NegateDispatchQueueCallback) {
    Mso::FutureWait(m_builderMock.Call1(
        L"NegateDispatchQueueCallback",
        std::function<void(int)>([](int result) noexcept { TestCheck(result == -4); }),
        4));
    TestCheck(m_builderMock.IsResolveCallbackCalled());
  }

  TEST_METHOD(TestMethodCall_NegateFutureCallback) {
    Mso::FutureWait(m_builderMock.Call1(
        L"NegateFutureCallback", std::function<void(int)>([](int result) noexcept { TestCheck(result == -4); }), 4));
    TestCheck(m_builderMock.IsResolveCallbackCalled());
  }

  TEST_METHOD(TestMethodCall_SayHelloCallback) {
    m_builderMock.Call1(L"SayHelloCallback", std::function<void(const std::string &)>([
                        ](const std::string &result) noexcept { TestCheck(result == "Hello_2"); }));
    TestCheck(m_builderMock.IsResolveCallbackCalled());
  }

  TEST_METHOD(TestMethodCall_StaticAddCallback) {
    m_builderMock.Call1(
        L"StaticAddCallback", std::function<void(int)>([](int result) noexcept { TestCheck(result == 60); }), 4, 56);
    TestCheck(m_builderMock.IsResolveCallbackCalled());
  }

  TEST_METHOD(TestMethodCall_StaticNegateCallback) {
    m_builderMock.Call1(
        L"StaticNegateCallback", std::function<void(int)>([](int result) noexcept { TestCheck(result == -33); }), 33);
    TestCheck(m_builderMock.IsResolveCallbackCalled());
  }

  TEST_METHOD(TestMethodCall_StaticNegateAsyncCallback) {
    Mso::FutureWait(m_builderMock.Call1(
        L"StaticNegateAsyncCallback",
        std::function<void(int)>([](int result) noexcept { TestCheck(result == -4); }),
        4));
    TestCheck(m_builderMock.IsResolveCallbackCalled());
  }

  TEST_METHOD(TestMethodCall_StaticNegateDispatchQueueCallback) {
    Mso::FutureWait(m_builderMock.Call1(
        L"StaticNegateDispatchQueueCallback",
        std::function<void(int)>([](int result) noexcept { TestCheck(result == -4); }),
        4));
    TestCheck(m_builderMock.IsResolveCallbackCalled());
  }

  TEST_METHOD(TestMethodCall_StaticNegateFutureCallback) {
    Mso::FutureWait(m_builderMock.Call1(
        L"StaticNegateFutureCallback",
        std::function<void(int)>([](int result) noexcept { TestCheck(result == -4); }),
        4));
    TestCheck(m_builderMock.IsResolveCallbackCalled());
  }

  TEST_METHOD(TestMethodCall_StaticSayHelloCallback) {
    m_builderMock.Call1(L"StaticSayHelloCallback", std::function<void(const std::string &)>([
                        ](const std::string &result) noexcept { TestCheck(result == "Static Hello_2"); }));
    TestCheck(m_builderMock.IsResolveCallbackCalled());
  }

  TEST_METHOD(TestMethodCall_DivideCallbacks) {
    m_builderMock.Call2(
        L"DivideCallbacks",
        std::function<void(int)>([](int result) noexcept { TestCheck(result == 3); }),
        std::function<void(std::string const &)>(
            [](std::string const &error) noexcept { TestCheck(error == "Division by 0"); }),
        6,
        2);
    TestCheck(m_builderMock.IsResolveCallbackCalled());
  }

  TEST_METHOD(TestMethodCall_DivideCallbacksError) {
    m_builderMock.Call2(
        L"DivideCallbacks",
        std::function<void(int)>([](int result) noexcept { TestCheck(result == 3); }),
        std::function<void(std::string const &)>(
            [](std::string const &error) noexcept { TestCheck(error == "Division by 0"); }),
        6,
        0);
    TestCheck(m_builderMock.IsRejectCallbackCalled());
  }

  TEST_METHOD(TestMethodCall_NegateCallbacks) {
    m_builderMock.Call2(
        L"NegateCallbacks",
        std::function<void(int)>([](int result) noexcept { TestCheck(result == -5); }),
        std::function<void(std::string const &)>(
            [](std::string const &error) noexcept { TestCheck(error == "Already negative"); }),
        5);
    TestCheck(m_builderMock.IsResolveCallbackCalled());
  }

  TEST_METHOD(TestMethodCall_NegateCallbacksError) {
    m_builderMock.Call2(
        L"NegateCallbacks",
        std::function<void(int)>([](int result) noexcept { TestCheck(result == -5); }),
        std::function<void(std::string const &)>(
            [](std::string const &error) noexcept { TestCheck(error == "Already negative"); }),
        -5);
    TestCheck(m_builderMock.IsRejectCallbackCalled());
  }

  TEST_METHOD(TestMethodCall_NegateAsyncCallbacks) {
    Mso::FutureWait(m_builderMock.Call2(
        L"NegateAsyncCallbacks",
        std::function<void(int)>([](int result) noexcept { TestCheck(result == -5); }),
        std::function<void(std::string const &)>(
            [](std::string const &error) noexcept { TestCheck(error == "Already negative"); }),
        5));
    TestCheck(m_builderMock.IsResolveCallbackCalled());
  }

  TEST_METHOD(TestMethodCall_NegateAsyncCallbacksError) {
    Mso::FutureWait(m_builderMock.Call2(
        L"NegateAsyncCallbacks",
        std::function<void(int)>([](int result) noexcept { TestCheck(result == -5); }),
        std::function<void(std::string const &)>(
            [](std::string const &error) noexcept { TestCheck(error == "Already negative"); }),
        -5));
    TestCheck(m_builderMock.IsRejectCallbackCalled());
  }

  TEST_METHOD(TestMethodCall_NegateDispatchQueueCallbacks) {
    Mso::FutureWait(m_builderMock.Call2(
        L"NegateDispatchQueueCallbacks",
        std::function<void(int)>([](int result) noexcept { TestCheck(result == -5); }),
        std::function<void(std::string const &)>(
            [](std::string const &error) noexcept { TestCheck(error == "Already negative"); }),
        5));
    TestCheck(m_builderMock.IsResolveCallbackCalled());
  }

  TEST_METHOD(TestMethodCall_NegateDispatchQueueCallbacksError) {
    Mso::FutureWait(m_builderMock.Call2(
        L"NegateDispatchQueueCallbacks",
        std::function<void(int)>([](int result) noexcept { TestCheck(result == -5); }),
        std::function<void(std::string const &)>(
            [](std::string const &error) noexcept { TestCheck(error == "Already negative"); }),
        -5));
    TestCheck(m_builderMock.IsRejectCallbackCalled());
  }

  TEST_METHOD(TestMethodCall_NegateFutureCallbacks) {
    Mso::FutureWait(m_builderMock.Call2(
        L"NegateFutureCallbacks",
        std::function<void(int)>([](int result) noexcept { TestCheck(result == -5); }),
        std::function<void(std::string const &)>(
            [](std::string const &error) noexcept { TestCheck(error == "Already negative"); }),
        5));
    TestCheck(m_builderMock.IsResolveCallbackCalled());
  }

  TEST_METHOD(TestMethodCall_NegateFutureCallbacksError) {
    Mso::FutureWait(m_builderMock.Call2(
        L"NegateFutureCallbacks",
        std::function<void(int)>([](int result) noexcept { TestCheck(result == -5); }),
        std::function<void(std::string const &)>(
            [](std::string const &error) noexcept { TestCheck(error == "Already negative"); }),
        -5));
    TestCheck(m_builderMock.IsRejectCallbackCalled());
  }

  TEST_METHOD(TestMethodCall_ResolveSayHelloCallbacks) {
    m_builderMock.Call2(
        L"ResolveSayHelloCallbacks",
        std::function<void(const std::string &)>(
            [](const std::string &result) noexcept { TestCheck(result == "Hello_3"); }),
        std::function<void(std::string const &)>(
            [](std::string const &error) noexcept { TestCheck(error == "Goodbye"); }));
    TestCheck(m_builderMock.IsResolveCallbackCalled());
  }

  TEST_METHOD(TestMethodCall_RejectSayHelloCallbacks) {
    m_builderMock.Call2(
        L"RejectSayHelloCallbacks",
        std::function<void(const std::string &)>(
            [](const std::string &result) noexcept { TestCheck(result == "Hello_3"); }),
        std::function<void(std::string const &)>(
            [](std::string const &error) noexcept { TestCheck(error == "Goodbye"); }));
    TestCheck(m_builderMock.IsRejectCallbackCalled());
  }

  TEST_METHOD(TestMethodCall_StaticDivideCallbacks) {
    m_builderMock.Call2(
        L"StaticDivideCallbacks",
        std::function<void(int)>([](int result) noexcept { TestCheck(result == 3); }),
        std::function<void(std::string const &)>(
            [](std::string const &error) noexcept { TestCheck(error == "Division by 0"); }),
        6,
        2);
    TestCheck(m_builderMock.IsResolveCallbackCalled());
  }

  TEST_METHOD(TestMethodCall_StaticDivideCallbacksError) {
    m_builderMock.Call2(
        L"StaticDivideCallbacks",
        std::function<void(int)>([](int result) noexcept { TestCheck(result == 3); }),
        std::function<void(std::string const &)>(
            [](std::string const &error) noexcept { TestCheck(error == "Division by 0"); }),
        6,
        0);
    TestCheck(m_builderMock.IsRejectCallbackCalled());
  }

  TEST_METHOD(TestMethodCall_StaticNegateCallbacks) {
    m_builderMock.Call2(
        L"StaticNegateCallbacks",
        std::function<void(int)>([](int result) noexcept { TestCheck(result == -5); }),
        std::function<void(std::string const &)>(
            [](std::string const &error) noexcept { TestCheck(error == "Already negative"); }),
        5);
    TestCheck(m_builderMock.IsResolveCallbackCalled());
  }

  TEST_METHOD(TestMethodCall_StaticNegateCallbacksError) {
    m_builderMock.Call2(
        L"StaticNegateCallbacks",
        std::function<void(int)>([](int result) noexcept { TestCheck(result == -5); }),
        std::function<void(std::string const &)>(
            [](std::string const &error) noexcept { TestCheck(error == "Already negative"); }),
        -5);
    TestCheck(m_builderMock.IsRejectCallbackCalled());
  }

  TEST_METHOD(TestMethodCall_StaticNegateAsyncCallbacks) {
    Mso::FutureWait(m_builderMock.Call2(
        L"StaticNegateAsyncCallbacks",
        std::function<void(int)>([](int result) noexcept { TestCheck(result == -5); }),
        std::function<void(std::string const &)>(
            [](std::string const &error) noexcept { TestCheck(error == "Already negative"); }),
        5));
    TestCheck(m_builderMock.IsResolveCallbackCalled());
  }

  TEST_METHOD(TestMethodCall_StaticNegateAsyncCallbacksError) {
    Mso::FutureWait(m_builderMock.Call2(
        L"StaticNegateAsyncCallbacks",
        std::function<void(int)>([](int result) noexcept { TestCheck(result == -5); }),
        std::function<void(std::string const &)>(
            [](std::string const &error) noexcept { TestCheck(error == "Already negative"); }),
        -5));
    TestCheck(m_builderMock.IsRejectCallbackCalled());
  }

  TEST_METHOD(TestMethodCall_StaticNegateDispatchQueueCallbacks) {
    Mso::FutureWait(m_builderMock.Call2(
        L"StaticNegateDispatchQueueCallbacks",
        std::function<void(int)>([](int result) noexcept { TestCheck(result == -5); }),
        std::function<void(std::string const &)>(
            [](std::string const &error) noexcept { TestCheck(error == "Already negative"); }),
        5));
    TestCheck(m_builderMock.IsResolveCallbackCalled());
  }

  TEST_METHOD(TestMethodCall_StaticNegateDispatchQueueCallbacksError) {
    Mso::FutureWait(m_builderMock.Call2(
        L"StaticNegateDispatchQueueCallbacks",
        std::function<void(int)>([](int result) noexcept { TestCheck(result == -5); }),
        std::function<void(std::string const &)>(
            [](std::string const &error) noexcept { TestCheck(error == "Already negative"); }),
        -5));
    TestCheck(m_builderMock.IsRejectCallbackCalled());
  }

  TEST_METHOD(TestMethodCall_StaticNegateFutureCallbacks) {
    Mso::FutureWait(m_builderMock.Call2(
        L"StaticNegateFutureCallbacks",
        std::function<void(int)>([](int result) noexcept { TestCheck(result == -5); }),
        std::function<void(std::string const &)>(
            [](std::string const &error) noexcept { TestCheck(error == "Already negative"); }),
        5));
    TestCheck(m_builderMock.IsResolveCallbackCalled());
  }

  TEST_METHOD(TestMethodCall_StaticNegateFutureCallbacksError) {
    Mso::FutureWait(m_builderMock.Call2(
        L"StaticNegateFutureCallbacks",
        std::function<void(int)>([](int result) noexcept { TestCheck(result == -5); }),
        std::function<void(std::string const &)>(
            [](std::string const &error) noexcept { TestCheck(error == "Already negative"); }),
        -5));
    TestCheck(m_builderMock.IsRejectCallbackCalled());
  }

  TEST_METHOD(TestMethodCall_StaticResolveSayHelloCallbacks) {
    m_builderMock.Call2(
        L"StaticResolveSayHelloCallbacks",
        std::function<void(const std::string &)>(
            [](const std::string &result) noexcept { TestCheck(result == "Hello_3"); }),
        std::function<void(std::string const &)>(
            [](std::string const &error) noexcept { TestCheck(error == "Goodbye"); }));
    TestCheck(m_builderMock.IsResolveCallbackCalled());
  }

  TEST_METHOD(TestMethodCall_StaticRejectSayHelloCallbacks) {
    m_builderMock.Call2(
        L"StaticRejectSayHelloCallbacks",
        std::function<void(const std::string &)>(
            [](const std::string &result) noexcept { TestCheck(result == "Hello_3"); }),
        std::function<void(std::string const &)>(
            [](std::string const &error) noexcept { TestCheck(error == "Goodbye"); }));
    TestCheck(m_builderMock.IsRejectCallbackCalled());
  }

  TEST_METHOD(TestMethodCall_DividePromise) {
    m_builderMock.Call2(
        L"DividePromise",
        std::function<void(int)>([](int result) noexcept { TestCheck(result == 3); }),
        std::function<void(JSValue const &)>(
            [](JSValue const &error) noexcept { TestCheck(error["message"] == "Division by 0"); }),
        6,
        2);
    TestCheck(m_builderMock.IsResolveCallbackCalled());
  }

  TEST_METHOD(TestMethodCall_DividePromiseError) {
    m_builderMock.Call2(
        L"DividePromise",
        std::function<void(int)>([](int result) noexcept { TestCheck(result == 3); }),
        std::function<void(JSValue const &)>(
            [](JSValue const &error) noexcept { TestCheck(error["message"] == "Division by 0"); }),
        6,
        0);
    TestCheck(m_builderMock.IsRejectCallbackCalled());
  }

  TEST_METHOD(TestMethodCall_NegatePromise) {
    m_builderMock.Call2(
        L"NegatePromise",
        std::function<void(int)>([](int result) noexcept { TestCheck(result == -5); }),
        std::function<void(JSValue const &)>(
            [](JSValue const &error) noexcept { TestCheck(error["message"] == "Already negative"); }),
        5);
    TestCheck(m_builderMock.IsResolveCallbackCalled());
  }

  TEST_METHOD(TestMethodCall_NegatePromiseError) {
    m_builderMock.Call2(
        L"NegatePromise",
        std::function<void(int)>([](int result) noexcept { TestCheck(result == -5); }),
        std::function<void(JSValue const &)>(
            [](JSValue const &error) noexcept { TestCheck(error["message"] == "Already negative"); }),
        -5);
    TestCheck(m_builderMock.IsRejectCallbackCalled());
  }

  TEST_METHOD(TestMethodCall_NegateAsyncPromise) {
    Mso::FutureWait(m_builderMock.Call2(
        L"NegateAsyncPromise",
        std::function<void(int)>([](int result) noexcept { TestCheck(result == -5); }),
        std::function<void(JSValue const &)>(
            [](JSValue const &error) noexcept { TestCheck(error["message"] == "Already negative"); }),
        5));
    TestCheck(m_builderMock.IsResolveCallbackCalled());
  }

  TEST_METHOD(TestMethodCall_NegateAsyncPromiseError) {
    Mso::FutureWait(m_builderMock.Call2(
        L"NegateAsyncPromise",
        std::function<void(int)>([](int result) noexcept { TestCheck(result == -5); }),
        std::function<void(JSValue const &)>(
            [](JSValue const &error) noexcept { TestCheck(error["message"] == "Already negative"); }),
        -5));
    TestCheck(m_builderMock.IsRejectCallbackCalled());
  }

  TEST_METHOD(TestMethodCall_NegateDispatchQueuePromise) {
    Mso::FutureWait(m_builderMock.Call2(
        L"NegateDispatchQueuePromise",
        std::function<void(int)>([](int result) noexcept { TestCheck(result == -5); }),
        std::function<void(JSValue const &)>(
            [](JSValue const &error) noexcept { TestCheck(error["message"] == "Already negative"); }),
        5));
    TestCheck(m_builderMock.IsResolveCallbackCalled());
  }

  TEST_METHOD(TestMethodCall_NegateDispatchQueuePromiseError) {
    Mso::FutureWait(m_builderMock.Call2(
        L"NegateDispatchQueuePromise",
        std::function<void(int)>([](int result) noexcept { TestCheck(result == -5); }),
        std::function<void(JSValue const &)>(
            [](JSValue const &error) noexcept { TestCheck(error["message"] == "Already negative"); }),
        -5));
    TestCheck(m_builderMock.IsRejectCallbackCalled());
  }

  TEST_METHOD(TestMethodCall_NegateFuturePromise) {
    Mso::FutureWait(m_builderMock.Call2(
        L"NegateFuturePromise",
        std::function<void(int)>([](int result) noexcept { TestCheck(result == -5); }),
        std::function<void(JSValue const &)>(
            [](JSValue const &error) noexcept { TestCheck(error["message"] == "Already negative"); }),
        5));
    TestCheck(m_builderMock.IsResolveCallbackCalled());
  }

  TEST_METHOD(TestMethodCall_NegateFuturePromiseError) {
    Mso::FutureWait(m_builderMock.Call2(
        L"NegateFuturePromise",
        std::function<void(int)>([](int result) noexcept { TestCheck(result == -5); }),
        std::function<void(JSValue const &)>(
            [](JSValue const &error) noexcept { TestCheck(error["message"] == "Already negative"); }),
        -5));
    TestCheck(m_builderMock.IsRejectCallbackCalled());
  }

  TEST_METHOD(TestMethodCall_VoidPromise) {
    m_builderMock.Call2(
        L"voidPromise",
        std::function<void()>([]() noexcept {}),
        std::function<void(JSValue const &)>(
            [](JSValue const &error) noexcept { TestCheck(error["message"] == "Odd unexpected"); }),
        2);
    TestCheck(m_builderMock.IsResolveCallbackCalled());
  }

  TEST_METHOD(TestMethodCall_VoidError) {
    m_builderMock.Call2(
        L"voidPromise",
        std::function<void()>([]() noexcept {}),
        std::function<void(JSValue const &)>(
            [](JSValue const &error) noexcept { TestCheck(error["message"] == "Odd unexpected"); }),
        3);
    TestCheck(m_builderMock.IsRejectCallbackCalled());
  }

  TEST_METHOD(TestMethodCall_ResolveSayHelloPromise) {
    m_builderMock.Call2(
        L"ResolveSayHelloPromise",
        std::function<void(const std::string &)>(
            [](const std::string &result) noexcept { TestCheck(result == "Hello_4"); }),
        std::function<void(JSValue const &)>(
            [](JSValue const &error) noexcept { TestCheck(error["message"] == "Promise rejected"); }));
    TestCheck(m_builderMock.IsResolveCallbackCalled());
  }

  TEST_METHOD(TestMethodCall_RejectSayHelloPromise) {
    m_builderMock.Call2(
        L"RejectSayHelloPromise",
        std::function<void(const std::string &)>(
            [](const std::string &result) noexcept { TestCheck(result == "Hello_4"); }),
        std::function<void(JSValue const &)>(
            [](JSValue const &error) noexcept { TestCheck(error["message"] == "Promise rejected"); }));
    TestCheck(m_builderMock.IsRejectCallbackCalled());
  }

  TEST_METHOD(TestMethodCall_StaticDividePromise) {
    m_builderMock.Call2(
        L"StaticDividePromise",
        std::function<void(int)>([](int result) noexcept { TestCheck(result == 3); }),
        std::function<void(JSValue const &)>(
            [](JSValue const &error) noexcept { TestCheck(error["message"] == "Division by 0"); }),
        6,
        2);
    TestCheck(m_builderMock.IsResolveCallbackCalled());
  }

  TEST_METHOD(TestMethodCall_StaticDividePromiseError) {
    m_builderMock.Call2(
        L"StaticDividePromise",
        std::function<void(int)>([](int result) noexcept { TestCheck(result == 3); }),
        std::function<void(JSValue const &)>(
            [](JSValue const &error) noexcept { TestCheck(error["message"] == "Division by 0"); }),
        6,
        0);
    TestCheck(m_builderMock.IsRejectCallbackCalled());
  }

  TEST_METHOD(TestMethodCall_StaticNegatePromise) {
    m_builderMock.Call2(
        L"StaticNegatePromise",
        std::function<void(int)>([](int result) noexcept { TestCheck(result == -5); }),
        std::function<void(JSValue const &)>(
            [](JSValue const &error) noexcept { TestCheck(error["message"] == "Already negative"); }),
        5);
    TestCheck(m_builderMock.IsResolveCallbackCalled());
  }

  TEST_METHOD(TestMethodCall_StaticNegatePromiseError) {
    Mso::FutureWait(m_builderMock.Call2(
        L"StaticNegateAsyncPromise",
        std::function<void(int)>([](int result) noexcept { TestCheck(result == -5); }),
        std::function<void(JSValue const &)>(
            [](JSValue const &error) noexcept { TestCheck(error["message"] == "Already negative"); }),
        -5));
    TestCheck(m_builderMock.IsRejectCallbackCalled());
  }

  TEST_METHOD(TestMethodCall_StaticNegateAsyncPromise) {
    Mso::FutureWait(m_builderMock.Call2(
        L"StaticNegateAsyncPromise",
        std::function<void(int)>([](int result) noexcept { TestCheck(result == -5); }),
        std::function<void(JSValue const &)>(
            [](JSValue const &error) noexcept { TestCheck(error["message"] == "Already negative"); }),
        5));
    TestCheck(m_builderMock.IsResolveCallbackCalled());
  }

  TEST_METHOD(TestMethodCall_StaticNegateAsyncPromiseError) {
    m_builderMock.Call2(
        L"StaticNegatePromise",
        std::function<void(int)>([](int result) noexcept { TestCheck(result == -5); }),
        std::function<void(JSValue const &)>(
            [](JSValue const &error) noexcept { TestCheck(error["message"] == "Already negative"); }),
        -5);
    TestCheck(m_builderMock.IsRejectCallbackCalled());
  }

  TEST_METHOD(TestMethodCall_StaticNegateDispatchQueuePromise) {
    Mso::FutureWait(m_builderMock.Call2(
        L"StaticNegateDispatchQueuePromise",
        std::function<void(int)>([](int result) noexcept { TestCheck(result == -5); }),
        std::function<void(JSValue const &)>(
            [](JSValue const &error) noexcept { TestCheck(error["message"] == "Already negative"); }),
        5));
    TestCheck(m_builderMock.IsResolveCallbackCalled());
  }

  TEST_METHOD(TestMethodCall_StaticNegateDispatchQueuePromiseError) {
    Mso::FutureWait(m_builderMock.Call2(
        L"StaticNegateDispatchQueuePromise",
        std::function<void(int)>([](int result) noexcept { TestCheck(result == -5); }),
        std::function<void(JSValue const &)>(
            [](JSValue const &error) noexcept { TestCheck(error["message"] == "Already negative"); }),
        -5));
    TestCheck(m_builderMock.IsRejectCallbackCalled());
  }

  TEST_METHOD(TestMethodCall_StaticNegateFuturePromise) {
    Mso::FutureWait(m_builderMock.Call2(
        L"StaticNegateFuturePromise",
        std::function<void(int)>([](int result) noexcept { TestCheck(result == -5); }),
        std::function<void(JSValue const &)>(
            [](JSValue const &error) noexcept { TestCheck(error["message"] == "Already negative"); }),
        5));
    TestCheck(m_builderMock.IsResolveCallbackCalled());
  }

  TEST_METHOD(TestMethodCall_StaticNegateFuturePromiseError) {
    Mso::FutureWait(m_builderMock.Call2(
        L"StaticNegateFuturePromise",
        std::function<void(int)>([](int result) noexcept { TestCheck(result == -5); }),
        std::function<void(JSValue const &)>(
            [](JSValue const &error) noexcept { TestCheck(error["message"] == "Already negative"); }),
        -5));
    TestCheck(m_builderMock.IsRejectCallbackCalled());
  }

  TEST_METHOD(TestMethodCall_StaticVoidPromise) {
    m_builderMock.Call2(
        L"staticVoidPromise",
        std::function<void()>([]() noexcept {}),
        std::function<void(JSValue const &)>(
            [](JSValue const &error) noexcept { TestCheck(error["message"] == "Odd unexpected"); }),
        2);
    TestCheck(m_builderMock.IsResolveCallbackCalled());
  }

  TEST_METHOD(TestMethodCall_StaticVoidPromiseError) {
    m_builderMock.Call2(
        L"staticVoidPromise",
        std::function<void()>([]() noexcept {}),
        std::function<void(JSValue const &)>(
            [](JSValue const &error) noexcept { TestCheck(error["message"] == "Odd unexpected"); }),
        3);
    TestCheck(m_builderMock.IsRejectCallbackCalled());
  }

  TEST_METHOD(TestMethodCall_StaticResolveSayHelloPromise) {
    m_builderMock.Call2(
        L"StaticResolveSayHelloPromise",
        std::function<void(const std::string &)>(
            [](const std::string &result) noexcept { TestCheck(result == "Hello_4"); }),
        std::function<void(JSValue const &)>(
            [](JSValue const &error) noexcept { TestCheck(error["message"] == "Promise rejected"); }));
    TestCheck(m_builderMock.IsResolveCallbackCalled());
  }

  TEST_METHOD(TestMethodCall_StaticRejectSayHelloPromise) {
    m_builderMock.Call2(
        L"StaticRejectSayHelloPromise",
        std::function<void(const std::string &)>(
            [](const std::string &result) noexcept { TestCheck(result == "Hello_4"); }),
        std::function<void(JSValue const &)>(
            [](JSValue const &error) noexcept { TestCheck(error["message"] == "Promise rejected"); }));
    TestCheck(m_builderMock.IsRejectCallbackCalled());
  }

  TEST_METHOD(TestMethodSyncCall_AddSync) {
    int result;
    m_builderMock.CallSync(L"AddSync", /*out*/ result, 3, 5);
    TestCheck(result == 8);
  }

  TEST_METHOD(TestMethodSyncCall_NegateSync) {
    int result;
    m_builderMock.CallSync(L"NegateSync", /*out*/ result, 7);
    TestCheck(result == -7);
  }

  TEST_METHOD(TestMethodSyncCall_SayHelloSync) {
    std::string result;
    m_builderMock.CallSync(L"SayHelloSync", /*out*/ result);
    TestCheck(result == "Hello");
  }

  TEST_METHOD(TestMethodSyncCall_StaticAddSync) {
    int result;
    m_builderMock.CallSync(L"StaticAddSync", /*out*/ result, 3, 5);
    TestCheck(result == 8);
  }

  TEST_METHOD(TestMethodSyncCall_StaticNegateSync) {
    int result;
    m_builderMock.CallSync(L"StaticNegateSync", /*out*/ result, 7);
    TestCheck(result == -7);
  }

  TEST_METHOD(TestMethodSyncCall_StaticSayHelloSync) {
    std::string result;
    m_builderMock.CallSync(L"StaticSayHelloSync", /*out*/ result);
    TestCheck(result == "Hello");
  }

  TEST_METHOD(TestConstants) {
    auto constants = m_builderMock.GetConstants();
    TestCheck(constants["Constant1"] == "MyConstant1");
    TestCheck(constants["const2"] == "MyConstant2");
    TestCheck(constants["const3"]["X"] == 2);
    TestCheck(constants["const3"]["Y"] == 3);
    TestCheck(constants["Constant4"]["X"] == 3);
    TestCheck(constants["Constant4"]["Y"] == 4);
    TestCheck(constants["const51"]["X"] == 12);
    TestCheck(constants["const51"]["Y"] == 14);
    TestCheck(constants["const52"] == "MyConstant52");
    TestCheck(constants["const61"]["X"] == 15);
    TestCheck(constants["const61"]["Y"] == 17);
    TestCheck(constants["const62"] == "MyConstant62");
  }

  TEST_METHOD(TestEvent_IntEventField) {
    bool eventRaised = false;
    m_builderMock.ExpectEvent(
        L"RCTDeviceEventEmitter", L"OnIntEvent", [&eventRaised](JSValueArray const &args) noexcept {
          TestCheck(args[0] == 42);
          eventRaised = true;
        });

    m_module->OnIntEvent(42);
    TestCheck(eventRaised);
  }

  TEST_METHOD(TestEvent_OnNoArgEventField) {
    bool eventRaised = false;
    m_builderMock.ExpectEvent(
        L"RCTDeviceEventEmitter", L"OnNoArgEvent", [&eventRaised](JSValueArray const &args) noexcept {
          TestCheckEqual(0, args.size());
          eventRaised = true;
        });

    m_module->OnNoArgEvent();
    TestCheck(eventRaised);
  }

  TEST_METHOD(TestEvent_TwoArgsEventField) {
    bool eventRaised = false;
    m_builderMock.ExpectEvent(
        L"RCTDeviceEventEmitter", L"OnTwoArgsEvent", [&eventRaised](JSValueArray const &args) noexcept {
          TestCheckEqual(4, args[0]["X"]);
          TestCheckEqual(2, args[0]["Y"]);
          TestCheckEqual(12, args[1]["X"]);
          TestCheckEqual(18, args[1]["Y"]);
          eventRaised = true;
        });

    m_module->OnTwoArgsEvent(Point2{/*X =*/4, /*Y =*/2}, Point2{/*X =*/12, /*Y =*/18});
    TestCheck(eventRaised);
  }

  TEST_METHOD(TestEvent_JSNameEventField) {
    bool eventRaised = false;
    m_builderMock.ExpectEvent(
        L"RCTDeviceEventEmitter", L"onPointEvent", [&eventRaised](JSValueArray const &args) noexcept {
          TestCheck(args[0]["X"] == 4);
          TestCheck(args[0]["Y"] == 2);
          eventRaised = true;
        });

    m_module->OnPointEvent(Point2{/*X =*/4, /*Y =*/2});
    TestCheck(eventRaised == true);
  }

  TEST_METHOD(TestEvent_JSEventEmitterEventField) {
    bool eventRaised = false;
    m_builderMock.ExpectEvent(L"MyEventEmitter", L"onStringEvent", [&eventRaised](JSValueArray const &args) noexcept {
      TestCheckEqual("Hello World!", args[0]);
      eventRaised = true;
    });

    m_module->OnStringEvent("Hello World!");
    TestCheck(eventRaised == true);
  }

  TEST_METHOD(TestEvent_JSValueObjectEventField) {
    bool eventRaised = false;
    m_builderMock.ExpectEvent(
        L"RCTDeviceEventEmitter", L"OnJSValueEvent", ([&eventRaised](JSValueArray const &args) noexcept {
          TestCheck(args[0]["X"] == 4);
          TestCheck(args[0]["Y"] == 2);
          eventRaised = true;
        }));

    m_module->OnJSValueEvent(JSValueObject{{"X", 4}, {"Y", 2}});
    TestCheck(eventRaised == true);
  }

  TEST_METHOD(TestEvent_JSValueArrayEventField) {
    bool eventRaised = false;
    m_builderMock.ExpectEvent(
        L"RCTDeviceEventEmitter", L"OnJSValueEvent", ([&eventRaised](JSValueArray const &args) noexcept {
          TestCheck(args[0][0] == "X");
          TestCheck(args[0][1] == 4);
          TestCheck(args[0][2] == true);
          TestCheck(args[0][3]["Id"] == 42);
          eventRaised = true;
        }));

    m_module->OnJSValueEvent(JSValueArray{"X", 4, true, JSValueObject{{"Id", 42}}});
    TestCheck(eventRaised == true);
  }

  TEST_METHOD(TestEvent_JSValueArray1EventField) {
    bool eventRaised = false;
    m_builderMock.ExpectEvent(
        L"RCTDeviceEventEmitter", L"OnJSValueEvent", ([&eventRaised](JSValueArray const &args) noexcept {
          TestCheck(args[0][0] == 4);
          eventRaised = true;
        }));

    m_module->OnJSValueEvent(JSValueArray{4});
    TestCheck(eventRaised == true);
  }

  TEST_METHOD(TestFunction_JSIntFunctionField) {
    bool functionCalled = false;
    m_builderMock.ExpectFunction(
        L"SimpleNativeModule2", L"JSIntFunction", [&functionCalled](JSValueArray const &args) noexcept {
          TestCheck(args[0] == 42);
          functionCalled = true;
        });

    m_module->JSIntFunction(42);
    TestCheck(functionCalled);
  }

  TEST_METHOD(TestFunction_JSNameFunctionField) {
    bool functionCalled = false;
    m_builderMock.ExpectFunction(
        L"SimpleNativeModule2", L"pointFunc", [&functionCalled](JSValueArray const &args) noexcept {
          TestCheck(args[0]["X"] == 4);
          TestCheck(args[0]["Y"] == 2);
          functionCalled = true;
        });

    m_module->JSPointFunction(Point2{/*X =*/4, /*Y =*/2});
    TestCheck(functionCalled == true);
  }

  TEST_METHOD(TestFunction_JSTwoArgFunctionField) {
    bool functionCalled = false;
    m_builderMock.ExpectFunction(
        L"SimpleNativeModule2", L"lineFunc", [&functionCalled](JSValueArray const &args) noexcept {
          TestCheck(args[0]["X"] == 4);
          TestCheck(args[0]["Y"] == 2);
          TestCheck(args[1]["X"] == 12);
          TestCheck(args[1]["Y"] == 18);
          functionCalled = true;
        });

    m_module->JSLineFunction(Point2{/*X =*/4, /*Y =*/2}, Point2{/*X =*/12, /*Y =*/18});
    TestCheck(functionCalled == true);
  }

  TEST_METHOD(TestFunction_JSNoArgFunctionField) {
    bool functionCalled = false;
    m_builderMock.ExpectFunction(
        L"SimpleNativeModule2", L"JSNoArgFunction", [&functionCalled](JSValueArray const &args) noexcept {
          TestCheckEqual(0, args.size());
          functionCalled = true;
        });

    m_module->JSNoArgFunction();
    TestCheck(functionCalled);
  }

  TEST_METHOD(TestFunction_JSModuleNameFunctionField) {
    bool functionCalled = false;
    m_builderMock.ExpectFunction(L"MyModule", L"stringFunc", [&functionCalled](JSValueArray const &args) noexcept {
      TestCheck(args[0] == "Hello World!");
      functionCalled = true;
    });

    m_module->JSStringFunction("Hello World!");
    TestCheck(functionCalled == true);
  }

  TEST_METHOD(TestFunction_JSValueObjectFunctionField) {
    bool functionCalled = false;
    m_builderMock.ExpectFunction(
        L"SimpleNativeModule2", L"JSValueFunction", ([&functionCalled](JSValueArray const &args) noexcept {
          TestCheck(args[0]["X"] == 4);
          TestCheck(args[0]["Y"] == 2);
          functionCalled = true;
        }));

    m_module->JSValueFunction(JSValueObject{{"X", 4}, {"Y", 2}});
    TestCheck(functionCalled == true);
  }

  TEST_METHOD(TestFunction_JSValueArrayFunctionField) {
    bool functionCalled = false;
    m_builderMock.ExpectFunction(
        L"SimpleNativeModule2", L"JSValueFunction", ([&functionCalled](JSValueArray const &args) noexcept {
          TestCheck(args[0][0] == "X");
          TestCheck(args[0][1] == 4);
          TestCheck(args[0][2] == true);
          TestCheck(args[0][3]["Id"] == 42);
          functionCalled = true;
        }));

    m_module->JSValueFunction(JSValueArray{"X", 4, true, JSValueObject{{"Id", 42}}});
    TestCheck(functionCalled == true);
  }

  TEST_METHOD(TestInitialized) {
    TestCheck(m_module->IsInitialized);
  }
};

} // namespace winrt::Microsoft::ReactNative
