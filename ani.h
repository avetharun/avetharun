// Android Native Interface : An interface for communicating with Java from native C++ applications built for Android.
// Licensing: See https://github.com/avetharun/avetharun/blob/main/gnu_gpl_3_a_LICENSE
#ifndef __rosemary_project_util_usb_h_
#define __rosemary_project_util_usb_h_
#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>
#include <jni.h>
#include <stdlib.h>
#include "utils.hpp"
size_t __curl_WriteStringCallback(char* ptr, size_t size, size_t nmemb);
struct ANIEnv {
	static inline const char* app_namespace = "com.app.name";
	static inline JNIEnv* env = 0;
	static inline JavaVM* vm = 0;
	static inline struct android_app* app_g = 0;
	static inline jobject clazz = 0;
	static int init(JavaVM* __vm, const char* app_namespace, jobject AndroidAppClazz, struct android_app* app) {
		int state_ = 0;
		ANIEnv::clazz = AndroidAppClazz;
		ANIEnv::vm = __vm;
		ANIEnv::app_namespace = app_namespace;
		state_ = vm->AttachCurrentThread(&ANIEnv::env, 0);
		ANIEnv::app_g = app;
		return state_;
	}
};
namespace ani {
	///Example reading values
	///SharedPreferences sharedPref(env,context,"pref_telemetry");
	///T_Protocol=sharedPref.getInt(IDT::T_Protocol);
	///Example writing values
	///SharedPreferences_Editor editor=sharedPref.edit();
	///editor.putString("MY_KEY","HELLO");
	///editor.commit();

	class SharedPreferences_Editor {
	public:
		SharedPreferences_Editor(JNIEnv* env, const jobject joSharedPreferences_Edit) :env(env), joSharedPreferences_Edit(joSharedPreferences_Edit) {
			//find the methods for putting values into Shared preferences via the editor
			jclass jcSharedPreferences_Editor = env->GetObjectClass(joSharedPreferences_Edit);
			jmPutBoolean = env->GetMethodID(jcSharedPreferences_Editor, "putBoolean", "(Ljava/lang/String;Z)Landroid/content/SharedPreferences$Editor;");
			jmPutInt = env->GetMethodID(jcSharedPreferences_Editor, "putInt", "(Ljava/lang/String;I)Landroid/content/SharedPreferences$Editor;");
			jmPutString = env->GetMethodID(jcSharedPreferences_Editor, "putString", "(Ljava/lang/String;Ljava/lang/String;)Landroid/content/SharedPreferences$Editor;");
			jmPutFloat = env->GetMethodID(jcSharedPreferences_Editor, "putFloat", "(Ljava/lang/String;F)Landroid/content/SharedPreferences$Editor;");
			jmPutLong = env->GetMethodID(jcSharedPreferences_Editor, "putLong", "(Ljava/lang/String;J)Landroid/content/SharedPreferences$Editor;");
			jmCommit = env->GetMethodID(jcSharedPreferences_Editor, "commit", "()Z");
		}
		//return itself for method chaining
		const SharedPreferences_Editor& putBoolean(const char* key, const bool value)const {
			env->CallObjectMethod(joSharedPreferences_Edit, jmPutBoolean, env->NewStringUTF(key), (jboolean)value);
			return *this;
		}
		const SharedPreferences_Editor& putInt(const char* key, const int value)const {
			env->CallObjectMethod(joSharedPreferences_Edit, jmPutInt, env->NewStringUTF(key), (jint)value);
			return *this;
		}
		const SharedPreferences_Editor& putString(const char* key, const char* value)const {
			env->CallObjectMethod(joSharedPreferences_Edit, jmPutString, env->NewStringUTF(key), env->NewStringUTF(value));
			return *this;
		}
		const SharedPreferences_Editor& putLong(const char* key, float value)const {
			env->CallObjectMethod(joSharedPreferences_Edit, jmPutLong, env->NewStringUTF(key), (jlong)value);
			return *this;
		}
		const SharedPreferences_Editor& putFloat(const char* key, float value)const {
			env->CallObjectMethod(joSharedPreferences_Edit, jmPutString, env->NewStringUTF(key), (jfloat)value);
			return *this;
		}
		bool commit()const {
			return (bool)env->CallBooleanMethod(joSharedPreferences_Edit, jmCommit);
		}
	private:
		JNIEnv* env;
		jobject joSharedPreferences_Edit;
		jmethodID jmPutBoolean;
		jmethodID jmPutFloat;
		jmethodID jmPutInt;
		jmethodID jmPutLong;
		jmethodID jmPutString;
		jmethodID jmCommit;
	};


	class SharedPreferences {
	public:
		SharedPreferences(SharedPreferences const&) = delete;
		void operator=(SharedPreferences const&) = delete;
	public:
		//Note: Per default, this doesn't keep the reference to the sharedPreferences java object alive
		//longer than the lifetime of the JNIEnv.
		//With keepReference=true the joSharedPreferences is kept 'alive' and you can still use the class after the original JNIEnv* has become invalid -
		//but make sure to refresh the JNIEnv* object with a new valid reference via replaceJNI()
		SharedPreferences(JNIEnv* env, jobject activity_clazz, const char* name, const bool keepReference = false) {
			this->env = env;
			//Find the 2 java classes we need to make calls with
			jclass jcContext = env->FindClass("android/content/Context");
			jclass jcSharedPreferences = env->FindClass("android/content/SharedPreferences");
			//jclass jcSharedPreferences_Editor=env->FindClass("android/content/SharedPreferences$Editor");
			if (jcContext == nullptr || jcSharedPreferences == nullptr) {
				__android_log_print(ANDROID_LOG_DEBUG, "SharedPreferences", "Cannot find classes");
			}
			//find the 3 functions we need to get values from an SharedPreferences instance
			jmGetBoolean = env->GetMethodID(jcSharedPreferences, "getBoolean", "(Ljava/lang/String;Z)Z");
			jmGetInt = env->GetMethodID(jcSharedPreferences, "getInt", "(Ljava/lang/String;I)I");
			jmGetLong = env->GetMethodID(jcSharedPreferences, "getLong", "(Ljava/lang/String;J)J");
			jmGetFloat = env->GetMethodID(jcSharedPreferences, "getFloat", "(Ljava/lang/String;F)F");
			jmGetString = env->GetMethodID(jcSharedPreferences, "getString", "(Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;");
			//find the 1 function we need to create the SharedPreferences.Editor object
			jmEdit = env->GetMethodID(jcSharedPreferences, "edit", "()Landroid/content/SharedPreferences$Editor;");
			//create a instance of SharedPreferences and store it in @joSharedPreferences
			jmethodID jmGetSharedPreferences = env->GetMethodID(jcContext, "getSharedPreferences", "(Ljava/lang/String;I)Landroid/content/SharedPreferences;");
			joSharedPreferences = env->CallObjectMethod(activity_clazz, jmGetSharedPreferences, env->NewStringUTF(name), MODE_PRIVATE);
			//jmEdit_commit=env->GetMethodID(jcSharedPreferences_Editor,"putString","(Ljava/lang/String;Ljava/lang/String;)Landroid/content/SharedPreferences$Editor;");
			if (keepReference) {
				joSharedPreferences = env->NewWeakGlobalRef(joSharedPreferences);
			}
		}
		void replaceJNI(JNIEnv* newEnv) {
			env = newEnv;
		}
	private:
		JNIEnv* env;
		jobject joSharedPreferences;
		jmethodID jmGetBoolean;
		jmethodID jmGetInt;
		jmethodID jmGetLong;
		jmethodID jmGetFloat;
		jmethodID jmGetString;
		jmethodID jmEdit;
	public:
		bool getBoolean(const char* id, bool defaultValue = false)const {
			return (bool)(env->CallBooleanMethod(joSharedPreferences, jmGetBoolean, env->NewStringUTF(id), (jboolean)defaultValue));
		}
		int getInt(const char* id, int defaultValue = 0)const {
			return (int)(env->CallIntMethod(joSharedPreferences, jmGetInt, env->NewStringUTF(id), (jint)defaultValue));
		}
		float getFloat(const char* id, float defaultValue = 0.0f)const {
			return (float)(env->CallFloatMethod(joSharedPreferences, jmGetFloat, env->NewStringUTF(id), (jfloat)defaultValue));
		}
		long getLong(const char* id, long defaultValue = 0)const {
			return (long)(env->CallFloatMethod(joSharedPreferences, jmGetLong, env->NewStringUTF(id), (jlong)defaultValue));
		}
		std::string getString(const char* id, const char* defaultValue = "")const {
			auto value = (jstring)(env->CallObjectMethod(joSharedPreferences, jmGetString, env->NewStringUTF(id), env->NewStringUTF(defaultValue)));
			const char* valueP = env->GetStringUTFChars(value, nullptr);
			const std::string ret = std::string(valueP);
			env->ReleaseStringUTFChars(value, valueP);
			//__android_log_print(ANDROID_LOG_DEBUG, "SharedPreferences","%s",ret.c_str());
			return ret;
		}
		void getBoolean(const char* id, bool* out, bool defaultValue = false)const {
			*out = (bool)(env->CallBooleanMethod(joSharedPreferences, jmGetBoolean, env->NewStringUTF(id), (jboolean)defaultValue));
		}
		void getInt(const char* id, int* out, int defaultValue = 0)const {
			*out = (int)(env->CallIntMethod(joSharedPreferences, jmGetInt, env->NewStringUTF(id), (jint)defaultValue));
		}
		void getFloat(const char* id, float* out, float defaultValue = 0.0f)const {
			*out = (float)(env->CallFloatMethod(joSharedPreferences, jmGetFloat, env->NewStringUTF(id), (jfloat)defaultValue));
		}
		void getLong(const char* id, long* out, long defaultValue = 0)const {
			*out = (long)(env->CallFloatMethod(joSharedPreferences, jmGetLong, env->NewStringUTF(id), (jlong)defaultValue));
		}
		void getString(const char* id, std::string* out, const char* defaultValue = "")const {
			auto value = (jstring)(env->CallObjectMethod(joSharedPreferences, jmGetString, env->NewStringUTF(id), env->NewStringUTF(defaultValue)));
			const char* valueP = env->GetStringUTFChars(value, nullptr);
			const std::string ret = std::string(valueP);
			env->ReleaseStringUTFChars(value, valueP);
			//__android_log_print(ANDROID_LOG_DEBUG, "SharedPreferences","%s",ret.c_str());
			*out = ret;
		}
		SharedPreferences_Editor edit()const {
			//create a instance of SharedPreferences.Editor and store it in @joSharedPreferences_Edit
			jobject joSharedPreferences_Edit = env->CallObjectMethod(joSharedPreferences, jmEdit);
			SharedPreferences_Editor editor(env, joSharedPreferences_Edit);
			return editor;
		}
	private:
		static constexpr const int  MODE_PRIVATE = 0; //taken directly from java, assuming this value stays constant in java
		static constexpr const int  MODE_WORLD_READABLE = 1; //taken directly from java, assuming this value stays constant in java
		static constexpr const int  MODE_WORLD_WRITEABLE = 2; //taken directly from java, assuming this value stays constant in java
	};

	// end shared prefs
	struct Networking {
		static void openURLBrowser(const char* url) {
			jstring url_string = ANIEnv::env->NewStringUTF(url);
			jclass uri_class = ANIEnv::env->FindClass("android/net/Uri");
			jmethodID uri_parse = ANIEnv::env->GetStaticMethodID(uri_class, "parse", "(Ljava/lang/String;)Landroid/net/Uri;");
			jobject uri = ANIEnv::env->CallStaticObjectMethod(uri_class, uri_parse, url_string);
			jclass intent_class = ANIEnv::env->FindClass("android/content/Intent");
			jfieldID action_view_id = ANIEnv::env->GetStaticFieldID(intent_class, "ACTION_VIEW", "Ljava/lang/String;");
			jobject action_view = ANIEnv::env->GetStaticObjectField(intent_class, action_view_id);
			jmethodID new_intent = ANIEnv::env->GetMethodID(intent_class, "<init>", "(Ljava/lang/String;Landroid/net/Uri;)V");
			jobject intent = ANIEnv::env->AllocObject(intent_class);
			ANIEnv::env->CallVoidMethod(intent, new_intent, action_view, uri);
			jclass activity_class = ANIEnv::env->FindClass("android/app/Activity");
			jmethodID start_activity = ANIEnv::env->GetMethodID(activity_class, "startActivity", "(Landroid/content/Intent;)V");
			ANIEnv::env->CallVoidMethod(ANIEnv::clazz, start_activity, intent);
		}
		static inline CURLcode last_curl_response = {};
		static inline bool _is_curl_init__ = false;
		static CURL* impl_curl_init() {
			if (!_is_curl_init__) {
				std::atexit(curl_global_cleanup);
				curl_global_init(CURL_GLOBAL_ALL);
				_is_curl_init__ = true;
			}
			return curl_easy_init();
		}
		static inline std::string __curl_request_data = "";
		static void ImplRequestInit() {
			__curl_request_data.clear();
		}
		static void sendPostRequestDataImpl__(const char* url, std::string body, std::string* response,
			std::list<std::string> headers = { "Content-Type: text/plain" }
		) {
			response->clear();
			curlpp::Cleanup curlCleanup;
			try {
				curlpp::Easy curlRequest;
				curlRequest.reset();
				curlpp::options::WriteFunction wf(__curl_WriteStringCallback);
				curlRequest.setOpt(wf);
				curlRequest.setOpt(new curlpp::options::HttpHeader(headers));
				curlRequest.setOpt(curlpp::options::Url(url));
				curlRequest.setOpt(new curlpp::options::PostFields(body));
				curlRequest.setOpt(new curlpp::options::PostFieldSize(body.length()));
				curlRequest.perform();
				*response = __curl_request_data;
			}
			catch (curlpp::LibcurlRuntimeError e) {
				*response = std::string(e.what()) + "| Curl returned : " + curl_easy_strerror(e.whatCode());
			}
			catch (curlpp::LogicError e) {
				*response = std::string(e.what());
			}
			ImplRequestInit();
		}
		static void ImplGetRequestHandler__(const char* url, std::string* response) {
			response->clear();
			curlpp::Cleanup curlCleanup;
			try {
				curlpp::Easy curlRequest;
				curlRequest.reset();
				curlpp::options::WriteFunction wf(__curl_WriteStringCallback);
				curlRequest.setOpt(wf);
				curlRequest.setOpt(curlpp::options::Url(url));
				curlRequest.perform();
				*response = __curl_request_data;
			}
			catch (curlpp::LibcurlRuntimeError e) {
				*response = std::string(e.what()) + "| Curl returned : " + curl_easy_strerror(e.whatCode());
			}
			catch (curlpp::LogicError e) {
				*response = std::string(e.what());
			}
			ImplRequestInit();
		}
		struct HeaderType {
#define ALIBHEADERTY static inline const std::list<std::string>			
			ALIBHEADERTY JSON = {
			  "Content-Type: application/json",
			  "accept: application/json"
			};
			ALIBHEADERTY PlainText = {
				"Content-Type: text/plain"
			};
		};
		// Assumes <response> will be valid upon request finished. Ensure it doesn't get deleted using shared_ptr or a static variable!
		// *response will automatically be cleared before the request is sent.
		static void sendGetRequestAsync(const char* url, std::string* response) {
			response->clear();
			std::thread t(ImplGetRequestHandler__, url, response);
			t.detach();
		}
		static std::string sendGetRequest(const char* url) {
			std::string out = "";
			ImplGetRequestHandler__(url, &out);
			return out;
		}
		// Assumes <response> will be valid upon request finished. Ensure it doesn't get deleted using shared_ptr or a static variable!
		// *response will automatically be cleared before the request is sent.
		static void sendPostRequestAsync(const char* url, std::string body, std::string* response, std::list<std::string> headers = { "Content-Type: text/plain" }) {
			response->clear();
			std::thread t(sendPostRequestDataImpl__, url, body, response, headers);
			t.detach();
		}
		static std::string sendPostRequest(const char* url, std::string body, std::list<std::string> headers = { "Content-Type: text/plain" }) {
			std::string out = "";
			sendPostRequestDataImpl__(url, body, &out, headers);
			return out;
		}


	};
	const char* __rootusb_class = "android.hardware.usb";
	struct SystemService {
		const char* rootcls = "android.content.Context";
		static inline jclass __clazz = 0;
		static jclass GetClass() {
			return __clazz = ANIEnv::env->FindClass("android.content.Context");
		}
		static jobject getSystemService(const char* service_type) {
			jmethodID __id = ANIEnv::env->GetMethodID(GetClass(), "getSystemService", "(Ljava/lang/String;)Ljava/lang/String;");
			jstring __s_arg = ANIEnv::env->NewStringUTF(service_type);
			jobject __sysobj = ANIEnv::env->CallStaticObjectMethod(__clazz, __id, __s_arg);
			ANIEnv::env->ReleaseStringUTFChars(__s_arg, service_type);
			return __sysobj;
		}
	};
	struct UsbAccessory {
		static jclass GetClass() {
			return ANIEnv::env->FindClass(alib_strfmt("%s.UsbAccessory", __rootusb_class));
		}
	};
	struct UsbInterface {
		static jclass GetClass() {
			return ANIEnv::env->FindClass(alib_strfmt("%s.UsbInterface", __rootusb_class));
		}
	};
	struct UsbDevice {
		static jclass GetClass() {
			return ANIEnv::env->FindClass(alib_strfmt("%s.UsbDevice", __rootusb_class));
		}
	};
	struct UsbManager {
		static jclass GetClass() {
			return ANIEnv::env->FindClass(alib_strfmt("%s.UsbManager", __rootusb_class));
		}
	};
};


size_t __curl_WriteStringCallback(char* ptr, size_t size, size_t nmemb)
{
	ani::Networking::__curl_request_data += std::string(ptr, size * nmemb);;
	int totalSize = size * nmemb;
	return totalSize;
}

#endif
