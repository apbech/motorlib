#ifndef PARAMETER_API_H
#define PARAMETER_API_H

#include <string>
#include <map>


class APIVariable {
 public:
   virtual std::string get() const = 0;
   virtual void set(std::string) = 0;
};

template<class T>
class APIVariable2 : public APIVariable {
 public:
   APIVariable2(T *value) : value_(value) {};
   APIVariable2(const T* value) : value_(const_cast<T*>(value)) {}
   virtual std::string get() const { return std::to_string(*value_); }
   virtual void set(std::string) = 0;
 protected:
   T *value_;
};

class APIFloat : public APIVariable2<float> {
 public:
   APIFloat(float *f) : APIVariable2(f) {}
   APIFloat(const  float *f) : APIVariable2(f) {}
   void set(std::string);
};

class APIUint32 : public APIVariable2<uint32_t> {
 public:
   APIUint32(uint32_t *u) : APIVariable2(u) {}
   APIUint32(const uint32_t *u) : APIVariable2(u) {}
   void set(std::string);
};

class APIInt32 : public APIVariable2<int32_t> {
 public:
   APIInt32(int32_t *u) : APIVariable2(u) {}
   void set(std::string);
};


class APIUint8 : public APIVariable2<uint8_t> {
 public:
   APIUint8(uint8_t *u) : APIVariable2(u) {}
   void set(std::string);
};

#include <functional>

class APICallback : public APIVariable {
 public:
  APICallback(std::function<std::string()> getfun, std::function<void(std::string)> setfun) : getfun_(getfun), setfun_(setfun) {}
  void set(std::string s) { setfun_(s); }
  std::string get() const {return getfun_(); }
 private:
  std::function<std::string()> getfun_;
  std::function<void(std::string)> setfun_;
};

class APICallbackFloat : public APIVariable {
 public:
   APICallbackFloat(std::function<float()> getfun , std::function<void(float)> setfun) : getfun_(getfun), setfun_(setfun) {}
   APICallbackFloat(std::function<float()> getfun) : getfun_(getfun) {}
   void set(std::string s) { setfun_(stof(s)); }
   std::string get() const { return std::to_string(getfun_()); };
 private:
   std::function<float()> getfun_;
   std::function<void(float)> setfun_;
};

class APICallbackUint32 : public APIVariable {
 public:
   APICallbackUint32(std::function<uint32_t()> getfun , std::function<void(uint32_t)> setfun) : getfun_(getfun), setfun_(setfun) {}
   APICallbackUint32(std::function<uint32_t()> getfun) : getfun_(getfun) {}
   void set(std::string);
   std::string get() const;
 private:
   std::function<uint32_t()> getfun_;
   std::function<void(uint32_t)> setfun_;
};

// allows for setting variables through text commands
class ParameterAPI {
 public:
    // type is used by scanf to parse the string
    void add_api_variable(std::string name, APIVariable *variable);
    void add_api_variable(std::string name, const APIVariable *variable);
    void set_api_variable(std::string name, std::string value);
    std::string get_api_variable(std::string name);
    std::string parse_string(std::string);
 private:
    std::map<std::string, APIVariable *> variable_map_;
    std::map<std::string, const APIVariable *> const_variable_map_;
    std::string last_string_;
};

#endif
