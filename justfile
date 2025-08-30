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

install: default
	install -v -m 755 mdtoc /usr/local/bin/.

clean:
	go clean
