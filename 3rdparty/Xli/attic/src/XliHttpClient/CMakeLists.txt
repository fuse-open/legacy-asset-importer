file(GLOB SRC
    ../../include/XliHttpClient/*.h
    ../../include/XliHttpClient.h
    Uri.cpp)

set(LIBS Xli XliPlatform)

if (XLI_PLATFORM_IOS)

    set(SRC ${SRC} ios/CHttpClient.mm)

elseif (XLI_PLATFORM_ANDROID)

    set(SRC ${SRC} android/AHttpClient.cpp)

else()
    if (WIN32)
        set(LIBCURL_SRC
            ../../3rdparty/curl/lib/file.c
            ../../3rdparty/curl/lib/timeval.c
            ../../3rdparty/curl/lib/base64.c
            ../../3rdparty/curl/lib/hostip.c
            ../../3rdparty/curl/lib/progress.c
            ../../3rdparty/curl/lib/formdata.c
            ../../3rdparty/curl/lib/cookie.c
            ../../3rdparty/curl/lib/http.c
            ../../3rdparty/curl/lib/sendf.c
            ../../3rdparty/curl/lib/ftp.c
            ../../3rdparty/curl/lib/url.c
            ../../3rdparty/curl/lib/dict.c
            ../../3rdparty/curl/lib/if2ip.c
            ../../3rdparty/curl/lib/speedcheck.c
            ../../3rdparty/curl/lib/ldap.c
            ../../3rdparty/curl/lib/version.c
            ../../3rdparty/curl/lib/getenv.c
            ../../3rdparty/curl/lib/escape.c
            ../../3rdparty/curl/lib/mprintf.c
            ../../3rdparty/curl/lib/telnet.c
            ../../3rdparty/curl/lib/netrc.c
            ../../3rdparty/curl/lib/getinfo.c
            ../../3rdparty/curl/lib/transfer.c
            ../../3rdparty/curl/lib/strequal.c
            ../../3rdparty/curl/lib/easy.c
            ../../3rdparty/curl/lib/security.c
            ../../3rdparty/curl/lib/curl_fnmatch.c
            ../../3rdparty/curl/lib/fileinfo.c
            ../../3rdparty/curl/lib/ftplistparser.c
            ../../3rdparty/curl/lib/wildcard.c
            ../../3rdparty/curl/lib/krb5.c
            ../../3rdparty/curl/lib/memdebug.c
            ../../3rdparty/curl/lib/http_chunks.c
            ../../3rdparty/curl/lib/strtok.c
            ../../3rdparty/curl/lib/connect.c
            ../../3rdparty/curl/lib/llist.c
            ../../3rdparty/curl/lib/hash.c
            ../../3rdparty/curl/lib/multi.c
            ../../3rdparty/curl/lib/content_encoding.c
            ../../3rdparty/curl/lib/share.c
            ../../3rdparty/curl/lib/http_digest.c
            ../../3rdparty/curl/lib/md4.c
            ../../3rdparty/curl/lib/md5.c
            ../../3rdparty/curl/lib/http_negotiate.c
            ../../3rdparty/curl/lib/inet_pton.c
            ../../3rdparty/curl/lib/strtoofft.c
            ../../3rdparty/curl/lib/strerror.c
            ../../3rdparty/curl/lib/amigaos.c
            ../../3rdparty/curl/lib/hostasyn.c
            ../../3rdparty/curl/lib/hostip4.c
            ../../3rdparty/curl/lib/hostip6.c
            ../../3rdparty/curl/lib/hostsyn.c
            ../../3rdparty/curl/lib/inet_ntop.c
            ../../3rdparty/curl/lib/parsedate.c
            ../../3rdparty/curl/lib/select.c
            ../../3rdparty/curl/lib/tftp.c
            ../../3rdparty/curl/lib/splay.c
            ../../3rdparty/curl/lib/strdup.c
            ../../3rdparty/curl/lib/socks.c
            ../../3rdparty/curl/lib/ssh.c
            ../../3rdparty/curl/lib/rawstr.c
            ../../3rdparty/curl/lib/curl_addrinfo.c
            ../../3rdparty/curl/lib/socks_gssapi.c
            ../../3rdparty/curl/lib/socks_sspi.c
            ../../3rdparty/curl/lib/curl_sspi.c
            ../../3rdparty/curl/lib/slist.c
            ../../3rdparty/curl/lib/nonblock.c
            ../../3rdparty/curl/lib/curl_memrchr.c
            ../../3rdparty/curl/lib/imap.c
            ../../3rdparty/curl/lib/pop3.c
            ../../3rdparty/curl/lib/smtp.c
            ../../3rdparty/curl/lib/pingpong.c
            ../../3rdparty/curl/lib/rtsp.c
            ../../3rdparty/curl/lib/curl_threads.c
            ../../3rdparty/curl/lib/warnless.c
            ../../3rdparty/curl/lib/hmac.c
            ../../3rdparty/curl/lib/curl_rtmp.c
            ../../3rdparty/curl/lib/openldap.c
            ../../3rdparty/curl/lib/curl_gethostname.c
            ../../3rdparty/curl/lib/gopher.c
            ../../3rdparty/curl/lib/idn_win32.c
            ../../3rdparty/curl/lib/http_negotiate_sspi.c
            ../../3rdparty/curl/lib/http_proxy.c
            ../../3rdparty/curl/lib/non-ascii.c
            ../../3rdparty/curl/lib/asyn-ares.c
            ../../3rdparty/curl/lib/asyn-thread.c
            ../../3rdparty/curl/lib/curl_gssapi.c
            ../../3rdparty/curl/lib/curl_ntlm.c
            ../../3rdparty/curl/lib/curl_ntlm_wb.c
            ../../3rdparty/curl/lib/curl_ntlm_core.c
            ../../3rdparty/curl/lib/curl_ntlm_msgs.c
            ../../3rdparty/curl/lib/curl_sasl.c
            ../../3rdparty/curl/lib/curl_multibyte.c
            ../../3rdparty/curl/lib/hostcheck.c
            ../../3rdparty/curl/lib/bundles.c
            ../../3rdparty/curl/lib/conncache.c
            ../../3rdparty/curl/lib/pipeline.c
            ../../3rdparty/curl/lib/dotdot.c
            ../../3rdparty/curl/lib/x509asn1.c
            ../../3rdparty/curl/lib/http2.c
            ../../3rdparty/curl/lib/curl_sasl_sspi.c
            ../../3rdparty/curl/lib/vtls/openssl.c
            ../../3rdparty/curl/lib/vtls/gtls.c
            ../../3rdparty/curl/lib/vtls/vtls.c
            ../../3rdparty/curl/lib/vtls/nss.c
            ../../3rdparty/curl/lib/vtls/polarssl.c
            ../../3rdparty/curl/lib/vtls/polarssl_threadlock.c
            ../../3rdparty/curl/lib/vtls/axtls.c
            ../../3rdparty/curl/lib/vtls/cyassl.c
            ../../3rdparty/curl/lib/vtls/curl_schannel.c
            ../../3rdparty/curl/lib/vtls/curl_darwinssl.c
            ../../3rdparty/curl/lib/vtls/gskit.c)

        set(SRC ${SRC} ${LIBCURL_SRC})
        include_directories(../../3rdparty/curl/include ../../3rdparty/curl/lib)
        add_definitions(-DCURL_STATICLIB -DUSE_IPV6 -DUSE_WINDOWS_SSPI -DUSE_SCHANNEL)
        set(CMAKE_STATIC_LINKER_FLAGS "ws2_32.lib wldap32.lib advapi32.lib")
    else()
        set(LIBS ${LIBS} curl)
    endif()

    set(SRC ${SRC} curl/CurlHttpClient.cpp)

endif()

add_library(XliHttpClient ${SRC})
target_link_libraries(XliHttpClient ${LIBS})
