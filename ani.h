// Android Native Interface : An interface for communicating with Java from native C++ applications built for Android.

// Licensing: See https://github.com/avetharun/avetharun/blob/main/gnu_gpl_3_a_LICENSE

#ifndef __rosemary_project_util_usb_h_
#define __rosemary_project_util_usb_h_
#include <jni.h>
#include "utils.hpp"
struct ANIEnv {
	static inline const char* app_namespace = "com.app.name";
	static inline JNIEnv* env = 0;
	static inline JavaVM* vm = 0;
	static inline jobject clazz = 0;
	static int init(JavaVM* __vm, const char* app_namespace, jobject AndroidAppClazz) {
		int state_ = 0;
		ANIEnv::clazz = AndroidAppClazz;
		ANIEnv::vm = __vm;
		ANIEnv::app_namespace = app_namespace;
		state_ = vm->AttachCurrentThread(&ANIEnv::env, 0);
		return state_;
	}
};
namespace ani {
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
		static void openURL(const char* url) {
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
#endif
