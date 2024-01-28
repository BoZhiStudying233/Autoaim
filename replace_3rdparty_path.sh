relative_path="/3rdparty/lib/"
absolute_path="$(pwd)$relative_path"

echo "$absolute_path"
# 当前目录
CURRENT_DIR="."

# 搜索并处理每个 CMakeLists.txt 文件
find $CURRENT_DIR -name 'CMakeLists.txt' | while read file; do
    # 检查文件中是否存在指定的模式
    if grep -q "set(3RDPARTY_LIB \"" $file; then
        # 替换找到的内容
        sed -i "s|set(3RDPARTY_LIB \".*\"|set(3RDPARTY_LIB \"$absolute_path\"|g" $file
        echo "Updated 3RDPARTY_LIB in $file"
    fi

done
