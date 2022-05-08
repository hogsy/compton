add_library(lisp STATIC
        private/lisp.cpp
        private/lisp_gc.cpp
        private/lisp_opt.cpp
        )

target_include_directories(lisp
        PUBLIC
        public/
        PRIVATE
        private/
        )

target_link_libraries(lisp
        plcore
        )
