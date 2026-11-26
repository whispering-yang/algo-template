# Build for local correctness checks and rerun the failing input.
g++ -std=c++17 -Og -g -Wall -Wextra -fno-omit-frame-pointer \
    -fsanitize=address,undefined -fno-sanitize-recover=undefined \
    -D_GLIBCXX_ASSERTIONS source.cpp -o debug
./debug < input.txt

# Rebuild for local timing with typical OJ optimization flags.
g++ -std=c++17 -O2 source.cpp -o program
