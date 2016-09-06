all : MathInterpTest.exe

MathInterpTest.exe : MathInterpTest.cpp cMathInterp.cpp cMathInterp.h cWordlist.cpp
	g++ MathInterpTest.cpp -o MathInterpTest.exe -static