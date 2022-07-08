/*
Autorzy:
Igor Witkowski
Monika Michaluk
*/

#ifndef ENCSTRSET_H
#define ENCSTRSET_H


#ifdef __cplusplus
    #include <cstddef>
    #include <iostream>
    extern "C" {
        namespace jnp1 {
#else
    #include <stddef.h>
    #include <stdbool.h>
#endif        
            unsigned long encstrset_new();
            
            void encstrset_delete(unsigned long id);
            
            size_t encstrset_size(unsigned long id);
            
            bool encstrset_insert(unsigned long id, const char* value, const char* key);
            
            bool encstrset_remove(unsigned long id, const char* value, const char* key);
            
            bool encstrset_test(unsigned long id, const char* value, const char* key);
            
            void encstrset_clear(unsigned long id);
            
            void encstrset_copy(unsigned long src_id, unsigned long dst_id);
            
#ifdef __cplusplus
    }
}
#endif

#endif /* ENCSTRSET_H */
