all:
	$(MAKE) -C ../rivemu rivemu.wasm
	cp ../rivemu/rivemu.{js,wasm} .
	find ../demos -name '*.sqfs' -exec cp {} cartridges/ \;
	find ../demos -name '*.rivcard' -exec cp {} cartridges/cards/ \;
	find ../demos -name '*.rivtape' -exec cp {} cartridges/tapes/ \;

clean:
	rm -f cartridges/*.sqfs
	rm -f cartridges/tapes/*.rivtape
	rm -f cartridges/cards/*.rivcard
	rm -f rivemu.{js,wasm}

serve:
	npx serve --cors
