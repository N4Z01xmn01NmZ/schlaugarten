#pragma once
#ifndef SCH_FIREBASE_H
#define SCH_FIREBASE_H

#define RTDB_PATH "device/"

namespace sch {

    void FirebaseInit(void);
    void FirebaseUpdateDB(void (*UpdateTask)(void));
    
    void FirebaseWriteInt(const char *path, int value);
    void FirebaseWriteFloat(const char *path, float value);
    void FirebaseWriteBool(const char *path, bool value); 
    
    void FirebaseReadInt(const char *path, int *value);
    void FirebaseReadFloat(const char *path, float *value);
    void FirebaseReadBool(const char *path, bool *value);

}

#endif /* SCH_FIREBASE_H */