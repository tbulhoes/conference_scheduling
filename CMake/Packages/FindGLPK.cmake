# - Try to find Glpk

find_package(Solver)

set(INCLUDE /usr/include/ 
			/cvos/shared/apps/imb/glpk/4.28/include
			/Program\ Files/GnuWin32/include
			/Program\ Files (X86)/GnuWin32/include)
set(LIBRARY /usr/lib64 
			/usr/lib
			/cvos/shared/apps/imb/glpk/4.28/lib
			/Program\ Files/GnuWin32/lib
			/Program\ Files (X86)/GnuWin32/lib)

find_solver(GLPK #Solver name
	    "" # number of version
	    glpk #Include name 
	    glpk #Library name
	    ${INCLUDE}
	    ${LIBRARY})