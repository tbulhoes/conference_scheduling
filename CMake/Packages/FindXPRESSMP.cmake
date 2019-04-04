# - Try to find Xpress

find_package(Solver)

set(INCLUDE /usr/include /opt/Xpress7.0-64/include)
set(LIBRARY /usr/lib64 /opt/Xpress7.0-64/lib /usr/lib /opt/Xpress7.0/lib)

find_solver(XPRESSMP #Solver name
	   "7.0"
	   xprs #Include name
	   xprs #Library name
	   ${INCLUDE}
	   ${LIBRARY})