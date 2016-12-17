
SRC=main.cpp StockProcessor.cpp util.cpp

stock: ${SRC}
	g++ -o stock ${SRC} -fpermissive \
	-lpthread

#	-Wc++11-compat-deprecated-writable-strings \

