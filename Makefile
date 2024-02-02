all:
	$(MAKE) -C ../rivemu rivemu.wasm
	cp ../rivemu/rivemu.{js,wasm} .
	find ../demos -name '*.sqfs' -exec cp {} cartridges/ \;

clean:
	rm -f cartridges/*.sqfs
	rm -f rivemu.{js,wasm}

serve:
	npx serve --cors
