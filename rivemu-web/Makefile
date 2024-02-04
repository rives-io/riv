all:
	$(MAKE) -C ../rivemu rivemu.js
	cp ../rivemu/rivemu.{js,wasm} .
	rm -rf cartridges
	mkdir -p cartridges
	find ../demos -name '*.sqfs' -exec cp {} cartridges/ \;
	emrun index.html

clean:
	rm -rf cartridges
	rm -f rivemu.{js,wasm}

publish:
	surge . rivemu.surge.sh
