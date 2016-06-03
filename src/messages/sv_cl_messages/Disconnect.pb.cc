// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: Disconnect.proto

#define INTERNAL_SUPPRESS_PROTOBUF_FIELD_DEPRECATION
#include "Disconnect.pb.h"

#include <algorithm>

#include <google/protobuf/stubs/common.h>
#include <google/protobuf/stubs/once.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/wire_format_lite_inl.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// @@protoc_insertion_point(includes)

namespace sv_cl_messages {

namespace {

const ::google::protobuf::Descriptor* Disconnect_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  Disconnect_reflection_ = NULL;

}  // namespace


void protobuf_AssignDesc_Disconnect_2eproto() {
  protobuf_AddDesc_Disconnect_2eproto();
  const ::google::protobuf::FileDescriptor* file =
    ::google::protobuf::DescriptorPool::generated_pool()->FindFileByName(
      "Disconnect.proto");
  GOOGLE_CHECK(file != NULL);
  Disconnect_descriptor_ = file->message_type(0);
  static const int Disconnect_offsets_[1] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Disconnect, reason_),
  };
  Disconnect_reflection_ =
    new ::google::protobuf::internal::GeneratedMessageReflection(
      Disconnect_descriptor_,
      Disconnect::default_instance_,
      Disconnect_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Disconnect, _has_bits_[0]),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Disconnect, _unknown_fields_),
      -1,
      ::google::protobuf::DescriptorPool::generated_pool(),
      ::google::protobuf::MessageFactory::generated_factory(),
      sizeof(Disconnect));
}

namespace {

GOOGLE_PROTOBUF_DECLARE_ONCE(protobuf_AssignDescriptors_once_);
inline void protobuf_AssignDescriptorsOnce() {
  ::google::protobuf::GoogleOnceInit(&protobuf_AssignDescriptors_once_,
                 &protobuf_AssignDesc_Disconnect_2eproto);
}

void protobuf_RegisterTypes(const ::std::string&) {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    Disconnect_descriptor_, &Disconnect::default_instance());
}

}  // namespace

void protobuf_ShutdownFile_Disconnect_2eproto() {
  delete Disconnect::default_instance_;
  delete Disconnect_reflection_;
}

void protobuf_AddDesc_Disconnect_2eproto() {
  static bool already_here = false;
  if (already_here) return;
  already_here = true;
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  ::google::protobuf::DescriptorPool::InternalAddGeneratedFile(
    "\n\020Disconnect.proto\022\016sv_cl_messages\"\034\n\nDi"
    "sconnect\022\016\n\006reason\030\001 \002(\t", 64);
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedFile(
    "Disconnect.proto", &protobuf_RegisterTypes);
  Disconnect::default_instance_ = new Disconnect();
  Disconnect::default_instance_->InitAsDefaultInstance();
  ::google::protobuf::internal::OnShutdown(&protobuf_ShutdownFile_Disconnect_2eproto);
}

// Force AddDescriptors() to be called at static initialization time.
struct StaticDescriptorInitializer_Disconnect_2eproto {
  StaticDescriptorInitializer_Disconnect_2eproto() {
    protobuf_AddDesc_Disconnect_2eproto();
  }
} static_descriptor_initializer_Disconnect_2eproto_;

// ===================================================================

#ifndef _MSC_VER
const int Disconnect::kReasonFieldNumber;
#endif  // !_MSC_VER

Disconnect::Disconnect()
  : ::google::protobuf::Message() {
  SharedCtor();
  // @@protoc_insertion_point(constructor:sv_cl_messages.Disconnect)
}

void Disconnect::InitAsDefaultInstance() {
}

Disconnect::Disconnect(const Disconnect& from)
  : ::google::protobuf::Message() {
  SharedCtor();
  MergeFrom(from);
  // @@protoc_insertion_point(copy_constructor:sv_cl_messages.Disconnect)
}

void Disconnect::SharedCtor() {
  ::google::protobuf::internal::GetEmptyString();
  _cached_size_ = 0;
  reason_ = const_cast< ::std::string*>(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

Disconnect::~Disconnect() {
  // @@protoc_insertion_point(destructor:sv_cl_messages.Disconnect)
  SharedDtor();
}

void Disconnect::SharedDtor() {
  if (reason_ != &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    delete reason_;
  }
  if (this != default_instance_) {
  }
}

void Disconnect::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* Disconnect::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return Disconnect_descriptor_;
}

const Disconnect& Disconnect::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_Disconnect_2eproto();
  return *default_instance_;
}

Disconnect* Disconnect::default_instance_ = NULL;

Disconnect* Disconnect::New() const {
  return new Disconnect;
}

void Disconnect::Clear() {
  if (has_reason()) {
    if (reason_ != &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
      reason_->clear();
    }
  }
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->Clear();
}

bool Disconnect::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) goto failure
  ::google::protobuf::uint32 tag;
  // @@protoc_insertion_point(parse_start:sv_cl_messages.Disconnect)
  for (;;) {
    ::std::pair< ::google::protobuf::uint32, bool> p = input->ReadTagWithCutoff(127);
    tag = p.first;
    if (!p.second) goto handle_unusual;
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // required string reason = 1;
      case 1: {
        if (tag == 10) {
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->mutable_reason()));
          ::google::protobuf::internal::WireFormat::VerifyUTF8StringNamedField(
            this->reason().data(), this->reason().length(),
            ::google::protobuf::internal::WireFormat::PARSE,
            "reason");
        } else {
          goto handle_unusual;
        }
        if (input->ExpectAtEnd()) goto success;
        break;
      }

      default: {
      handle_unusual:
        if (tag == 0 ||
            ::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_END_GROUP) {
          goto success;
        }
        DO_(::google::protobuf::internal::WireFormat::SkipField(
              input, tag, mutable_unknown_fields()));
        break;
      }
    }
  }
success:
  // @@protoc_insertion_point(parse_success:sv_cl_messages.Disconnect)
  return true;
failure:
  // @@protoc_insertion_point(parse_failure:sv_cl_messages.Disconnect)
  return false;
#undef DO_
}

void Disconnect::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // @@protoc_insertion_point(serialize_start:sv_cl_messages.Disconnect)
  // required string reason = 1;
  if (has_reason()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8StringNamedField(
      this->reason().data(), this->reason().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE,
      "reason");
    ::google::protobuf::internal::WireFormatLite::WriteStringMaybeAliased(
      1, this->reason(), output);
  }

  if (!unknown_fields().empty()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
  // @@protoc_insertion_point(serialize_end:sv_cl_messages.Disconnect)
}

::google::protobuf::uint8* Disconnect::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // @@protoc_insertion_point(serialize_to_array_start:sv_cl_messages.Disconnect)
  // required string reason = 1;
  if (has_reason()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8StringNamedField(
      this->reason().data(), this->reason().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE,
      "reason");
    target =
      ::google::protobuf::internal::WireFormatLite::WriteStringToArray(
        1, this->reason(), target);
  }

  if (!unknown_fields().empty()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        unknown_fields(), target);
  }
  // @@protoc_insertion_point(serialize_to_array_end:sv_cl_messages.Disconnect)
  return target;
}

int Disconnect::ByteSize() const {
  int total_size = 0;

  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // required string reason = 1;
    if (has_reason()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::StringSize(
          this->reason());
    }

  }
  if (!unknown_fields().empty()) {
    total_size +=
      ::google::protobuf::internal::WireFormat::ComputeUnknownFieldsSize(
        unknown_fields());
  }
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = total_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void Disconnect::MergeFrom(const ::google::protobuf::Message& from) {
  GOOGLE_CHECK_NE(&from, this);
  const Disconnect* source =
    ::google::protobuf::internal::dynamic_cast_if_available<const Disconnect*>(
      &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void Disconnect::MergeFrom(const Disconnect& from) {
  GOOGLE_CHECK_NE(&from, this);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from.has_reason()) {
      set_reason(from.reason());
    }
  }
  mutable_unknown_fields()->MergeFrom(from.unknown_fields());
}

void Disconnect::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void Disconnect::CopyFrom(const Disconnect& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool Disconnect::IsInitialized() const {
  if ((_has_bits_[0] & 0x00000001) != 0x00000001) return false;

  return true;
}

void Disconnect::Swap(Disconnect* other) {
  if (other != this) {
    std::swap(reason_, other->reason_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.Swap(&other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::google::protobuf::Metadata Disconnect::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = Disconnect_descriptor_;
  metadata.reflection = Disconnect_reflection_;
  return metadata;
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace sv_cl_messages

// @@protoc_insertion_point(global_scope)
