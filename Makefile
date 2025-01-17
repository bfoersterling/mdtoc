all:
	go build .

run:
	go run . test_files/test.md

test:
	go run . test_files/test.md
	go run . test_files/README.md
	go run . test_files/weird_headers.md
	go run . test_files/audio.md

install:
	cp -v mdtoc /usr/local/bin/.

clean:
	go clean
