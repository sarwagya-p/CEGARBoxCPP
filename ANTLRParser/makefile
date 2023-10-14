JAR=antlr-4.13.1-complete.jar

antlr:
	mkdir -p src/
	java -jar $(JAR) -Dlanguage=Cpp -o src/ FormulaK.g4 -no-listener -visitor

clean:
	rm -rf src/