## 安装依赖 
安装 `vcpkg`，然后使用vcpkg安装下面的依赖：

```shell
./vcpkg install spdlog
```

## 编译
```shell
mkdir build && cd build
cmake .. --toolchain /path/to/vcpkg.cmake
cmake --build . -j4
```

## 测试运行
```
cd build
./bin/TotpExample -i ../Examples/Segment1.txt
```