

typedef struct{
	int (*split_string_n)(int max,char *str, char **result, const char *delimiters) ;
	int (*findstr)( char* haystack,char* needle  );
	int (*intercept)( char* src,char* dest,char* sstr,char* estr );
}stropt_t;

extern stropt_t* stropt;
