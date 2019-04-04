# - Define some properties for the project, not really used yet
# - But it could be powerfull, because you can describe propertie 
#   with any scope you want

# The name of the executable
define_property(GLOBAL 
		PROPERTY USER_OUTPUT_NAME 
		BRIEF_DOCS "User name of the output" 
		FULL_DOCS "User name of the output define for each module")

# The additionals libraries link to the executable by the user
define_property(GLOBAL
		PROPERTY USER_LIBRARIES
		BRIEF_DOCS "Additional libraries" 
		FULL_DOCS "User's libraries add for one module")
