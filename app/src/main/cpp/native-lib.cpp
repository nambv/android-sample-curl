#include <jni.h>
#include <string>
#include <curl.h>
#include "Apis.h"
#include <iostream>

extern "C"
JNIEXPORT jstring JNICALL
Java_com_consciousapps_cmakecurltest_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string response = loginApi();
    return env->NewStringUTF(response.c_str());
}


static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    ((std::string *) userp)->append((char *) contents, size * nmemb);
    return size * nmemb;
}

std::string loginApi() {

    std::string readBuffer;
    long responseCode = 0;

    CURL *curl;
    CURLcode res;

    char* jsonBody = const_cast<char *>(R"({ "name" : "Pedro" , "age" : "22" })");

    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "Accept: application/json");
    headers = curl_slist_append(headers, "Content-Type: application/json");
    headers = curl_slist_append(headers, "charsets: utf-8");

    curl_global_init(CURL_GLOBAL_ALL);

    curl = curl_easy_init();

    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, "https://randomuser.me/api");
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false);
//        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
//        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonBody);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

        res = curl_easy_perform(curl);

        /* Check for errors */
        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                    curl_easy_strerror(res));
        } else {
            curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &responseCode);
        }

        /* always cleanup */
        curl_easy_cleanup(curl);
    }
    curl_global_cleanup();

    std::cout << readBuffer << std::endl;
    std::cout << responseCode << std::endl;

    return responseCode + "\n" + readBuffer;
}