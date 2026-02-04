FROM archlinux:latest

# needed for valgrind
ENV DEBUGINFOD_URLS="https://debuginfod.archlinux.org"

RUN pacman -Sy --noconfirm
# need latest glibc version for valgrind
RUN pacman -S glibc --noconfirm
RUN pacman -S check clang cmark make valgrind --noconfirm
