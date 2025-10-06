# 内置

## 运算符函数

在运算符操作中被调用，不能以函数形式被调用。

## operator[]=

## operator+

## operator-

## -operator

## operator*

## operator/

## operator^

## operator==

## operator!=

## operator>

## operator>=

## operator<

## operator<=

## operator@and

## operator@or

## operator@not

## 可被重载的内置函数

不可直接调用，内置函数运行时可能调用这些函数，重载后会影响某些内置函数的行为。

### @write

打印单个对象

### @boolean

转为boolean

### @integer

转为integer

### @decimal

转为decimal

### @character

转为character

## 重载后不会生效的函数名

这些函数会匹配所有可能的参数列表，需要通过重载特殊内置函数来修改其行为。

### print

打印并换行

### printr

打印不换行