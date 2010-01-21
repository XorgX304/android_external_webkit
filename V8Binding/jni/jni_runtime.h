/*
 * Copyright 2010, The Android Open Source Project
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE COMPUTER, INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE COMPUTER, INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef jni_runtime_h
#define jni_runtime_h

#include "JNIUtility.h"
#include "JavaInstanceV8.h"

#if USE(V8)
#include "JavaStringV8.h"
#endif

namespace JSC {

namespace Bindings {

class JavaString {
public:
    JavaString()
    {
        m_impl.init();
    }

    JavaString(JNIEnv* e, jstring s)
    {
        m_impl.init(e, s);
    }

    JavaString(jstring s)
    {
        m_impl.init(getJNIEnv(), s);
    }

    const char* UTF8String() const { return m_impl.UTF8String(); }
    const jchar* uchars() const { return m_impl.uchars(); }
    int length() const { return m_impl.length(); }

private:
    JavaStringImpl m_impl;
};

class JavaParameter {
public:
    JavaParameter() : m_JNIType(invalid_type) { }
    JavaParameter(JNIEnv*, jstring type);
    virtual ~JavaParameter() { }

    const char* type() const { return m_type.UTF8String(); }
    JNIType getJNIType() const { return m_JNIType; }

private:
    JavaString m_type;
    JNIType m_JNIType;
};


class JavaField {
public:
    JavaField(JNIEnv*, jobject aField);

    const JavaString& name() const { return m_name; }
    const char* type() const { return m_type.UTF8String(); }

    JNIType getJNIType() const { return m_JNIType; }

private:
    JavaString m_name;
    JavaString m_type;
    JNIType m_JNIType;
    RefPtr<JObjectWrapper> m_field;
};


class JavaMethod {
public:
    JavaMethod(JNIEnv*, jobject aMethod);
    ~JavaMethod();

    const JavaString& name() const { return m_name; }
    const char* returnType() const { return m_returnType.UTF8String(); }
    JavaParameter* parameterAt(int i) const { return &m_parameters[i]; }
    int numParameters() const { return m_numParameters; }

    const char* signature() const;
    JNIType JNIReturnType() const;

    jmethodID methodID(jobject obj) const;

    bool isStatic() const { return m_isStatic; }

private:
    JavaParameter* m_parameters;
    int m_numParameters;
    JavaString m_name;
    mutable char* m_signature;
    JavaString m_returnType;
    JNIType m_JNIReturnType;
    mutable jmethodID m_methodID;
    bool m_isStatic;
};

} // namespace Bindings

} // namespace JSC

#endif // jni_runtime_h
