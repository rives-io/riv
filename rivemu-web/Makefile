
all:
	$(MAKE) -C ../rivemu rivemu.js
	cp ../rivemu/rivemu.{js,wasm} .
	rm -rf cartridges
	mkdir -p cartridges
	find ../demos -name '*.sqfs' -exec cp {} cartridges/ \;
	emrun rivemu.html
