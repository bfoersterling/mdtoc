default:
	@just -l

build:
	go build .

run:
	go run . test_files/test.md

test:
	go test -v

bench:
	go test -v -bench="."

install: build
	install -v -m 755 mdtoc /usr/local/bin/.

install_latest_release:
	wget https://github.com/bfoersterling/mdtoc/releases/latest/download/mdtoc_linux_amd64 -O /tmp/mdtoc
	sudo install -v -m 755 /tmp/mdtoc /usr/local/bin

clean:
	go clean
