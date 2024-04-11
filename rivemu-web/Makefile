all:
	$(MAKE) -C ../rivemu rivemu-wasm
	cp ../rivemu/rivemu.{js,wasm} .
	mkdir -p cartridges
	find ../demos -name '*.sqfs' -exec cp {} cartridges/ \;
	emrun index.html

clean:
	rm -rf cartridges
	rm -f rivemu.{js,wasm}

publish:
	surge . rivemu.surge.sh
