// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: NetTables.proto

#define INTERNAL_SUPPRESS_PROTOBUF_FIELD_DEPRECATION
#include "NetTables.pb.h"

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

const ::google::protobuf::Descriptor* NetTables_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  NetTables_reflection_ = NULL;
const ::google::protobuf::Descriptor* NetTables_Table_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  NetTables_Table_reflection_ = NULL;

}  // namespace


void protobuf_AssignDesc_NetTables_2eproto() {
  protobuf_AddDesc_NetTables_2eproto();
  const ::google::protobuf::FileDescriptor* file =
    ::google::protobuf::DescriptorPool::generated_pool()->FindFileByName(
      "NetTables.proto");
  GOOGLE_CHECK(file != NULL);
  NetTables_descriptor_ = file->message_type(0);
  static const int NetTables_offsets_[1] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(NetTables, tables_),
  };
  NetTables_reflection_ =
    new ::google::protobuf::internal::GeneratedMessageReflection(
      NetTables_descriptor_,
      NetTables::default_instance_,
      NetTables_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(NetTables, _has_bits_[0]),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(NetTables, _unknown_fields_),
      -1,
      ::google::protobuf::DescriptorPool::generated_pool(),
      ::google::protobuf::MessageFactory::generated_factory(),
      sizeof(NetTables));
  NetTables_Table_descriptor_ = NetTables_descriptor_->nested_type(0);
  static const int NetTables_Table_offsets_[2] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(NetTables_Table, name_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(NetTables_Table, maxentries_),
  };
  NetTables_Table_reflection_ =
    new ::google::protobuf::internal::GeneratedMessageReflection(
      NetTables_Table_descriptor_,
      NetTables_Table::default_instance_,
      NetTables_Table_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(NetTables_Table, _has_bits_[0]),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(NetTables_Table, _unknown_fields_),
      -1,
      ::google::protobuf::DescriptorPool::generated_pool(),
      ::google::protobuf::MessageFactory::generated_factory(),
      sizeof(NetTables_Table));
}

namespace {

GOOGLE_PROTOBUF_DECLARE_ONCE(protobuf_AssignDescriptors_once_);
inline void protobuf_AssignDescriptorsOnce() {
  ::google::protobuf::GoogleOnceInit(&protobuf_AssignDescriptors_once_,
                 &protobuf_AssignDesc_NetTables_2eproto);
}

void protobuf_RegisterTypes(const ::std::string&) {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    NetTables_descriptor_, &NetTables::default_instance());
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    NetTables_Table_descriptor_, &NetTables_Table::default_instance());
}

}  // namespace

void protobuf_ShutdownFile_NetTables_2eproto() {
  delete NetTables::default_instance_;
  delete NetTables_reflection_;
  delete NetTables_Table::default_instance_;
  delete NetTables_Table_reflection_;
}

void protobuf_AddDesc_NetTables_2eproto() {
  static bool already_here = false;
  if (already_here) return;
  already_here = true;
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  ::google::protobuf::DescriptorPool::InternalAddGeneratedFile(
    "\n\017NetTables.proto\022\016sv_cl_messages\"g\n\tNet"
    "Tables\022/\n\006tables\030\001 \003(\0132\037.sv_cl_messages."
    "NetTables.Table\032)\n\005Table\022\014\n\004name\030\001 \002(\t\022\022"
    "\n\nmaxEntries\030\002 \002(\r", 138);
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedFile(
    "NetTables.proto", &protobuf_RegisterTypes);
  NetTables::default_instance_ = new NetTables();
  NetTables_Table::default_instance_ = new NetTables_Table();
  NetTables::default_instance_->InitAsDefaultInstance();
  NetTables_Table::default_instance_->InitAsDefaultInstance();
  ::google::protobuf::internal::OnShutdown(&protobuf_ShutdownFile_NetTables_2eproto);
}

// Force AddDescriptors() to be called at static initialization time.
struct StaticDescriptorInitializer_NetTables_2eproto {
  StaticDescriptorInitializer_NetTables_2eproto() {
    protobuf_AddDesc_NetTables_2eproto();
  }
} static_descriptor_initializer_NetTables_2eproto_;

// ===================================================================

#ifndef _MSC_VER
const int NetTables_Table::kNameFieldNumber;
const int NetTables_Table::kMaxEntriesFieldNumber;
#endif  // !_MSC_VER

NetTables_Table::NetTables_Table()
  : ::google::protobuf::Message() {
  SharedCtor();
  // @@protoc_insertion_point(constructor:sv_cl_messages.NetTables.Table)
}

void NetTables_Table::InitAsDefaultInstance() {
}

NetTables_Table::NetTables_Table(const NetTables_Table& from)
  : ::google::protobuf::Message() {
  SharedCtor();
  MergeFrom(from);
  // @@protoc_insertion_point(copy_constructor:sv_cl_messages.NetTables.Table)
}

void NetTables_Table::SharedCtor() {
  ::google::protobuf::internal::GetEmptyString();
  _cached_size_ = 0;
  name_ = const_cast< ::std::string*>(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  maxentries_ = 0u;
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

NetTables_Table::~NetTables_Table() {
  // @@protoc_insertion_point(destructor:sv_cl_messages.NetTables.Table)
  SharedDtor();
}

void NetTables_Table::SharedDtor() {
  if (name_ != &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    delete name_;
  }
  if (this != default_instance_) {
  }
}

void NetTables_Table::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* NetTables_Table::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return NetTables_Table_descriptor_;
}

const NetTables_Table& NetTables_Table::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_NetTables_2eproto();
  return *default_instance_;
}

NetTables_Table* NetTables_Table::default_instance_ = NULL;

NetTables_Table* NetTables_Table::New() const {
  return new NetTables_Table;
}

void NetTables_Table::Clear() {
  if (_has_bits_[0 / 32] & 3) {
    if (has_name()) {
      if (name_ != &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
        name_->clear();
      }
    }
    maxentries_ = 0u;
  }
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->Clear();
}

bool NetTables_Table::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) goto failure
  ::google::protobuf::uint32 tag;
  // @@protoc_insertion_point(parse_start:sv_cl_messages.NetTables.Table)
  for (;;) {
    ::std::pair< ::google::protobuf::uint32, bool> p = input->ReadTagWithCutoff(127);
    tag = p.first;
    if (!p.second) goto handle_unusual;
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // required string name = 1;
      case 1: {
        if (tag == 10) {
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->mutable_name()));
          ::google::protobuf::internal::WireFormat::VerifyUTF8StringNamedField(
            this->name().data(), this->name().length(),
            ::google::protobuf::internal::WireFormat::PARSE,
            "name");
        } else {
          goto handle_unusual;
        }
        if (input->ExpectTag(16)) goto parse_maxEntries;
        break;
      }

      // required uint32 maxEntries = 2;
      case 2: {
        if (tag == 16) {
         parse_maxEntries:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::uint32, ::google::protobuf::internal::WireFormatLite::TYPE_UINT32>(
                 input, &maxentries_)));
          set_has_maxentries();
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
  // @@protoc_insertion_point(parse_success:sv_cl_messages.NetTables.Table)
  return true;
failure:
  // @@protoc_insertion_point(parse_failure:sv_cl_messages.NetTables.Table)
  return false;
#undef DO_
}

void NetTables_Table::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // @@protoc_insertion_point(serialize_start:sv_cl_messages.NetTables.Table)
  // required string name = 1;
  if (has_name()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8StringNamedField(
      this->name().data(), this->name().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE,
      "name");
    ::google::protobuf::internal::WireFormatLite::WriteStringMaybeAliased(
      1, this->name(), output);
  }

  // required uint32 maxEntries = 2;
  if (has_maxentries()) {
    ::google::protobuf::internal::WireFormatLite::WriteUInt32(2, this->maxentries(), output);
  }

  if (!unknown_fields().empty()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
  // @@protoc_insertion_point(serialize_end:sv_cl_messages.NetTables.Table)
}

::google::protobuf::uint8* NetTables_Table::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // @@protoc_insertion_point(serialize_to_array_start:sv_cl_messages.NetTables.Table)
  // required string name = 1;
  if (has_name()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8StringNamedField(
      this->name().data(), this->name().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE,
      "name");
    target =
      ::google::protobuf::internal::WireFormatLite::WriteStringToArray(
        1, this->name(), target);
  }

  // required uint32 maxEntries = 2;
  if (has_maxentries()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteUInt32ToArray(2, this->maxentries(), target);
  }

  if (!unknown_fields().empty()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        unknown_fields(), target);
  }
  // @@protoc_insertion_point(serialize_to_array_end:sv_cl_messages.NetTables.Table)
  return target;
}

int NetTables_Table::ByteSize() const {
  int total_size = 0;

  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // required string name = 1;
    if (has_name()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::StringSize(
          this->name());
    }

    // required uint32 maxEntries = 2;
    if (has_maxentries()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::UInt32Size(
          this->maxentries());
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

void NetTables_Table::MergeFrom(const ::google::protobuf::Message& from) {
  GOOGLE_CHECK_NE(&from, this);
  const NetTables_Table* source =
    ::google::protobuf::internal::dynamic_cast_if_available<const NetTables_Table*>(
      &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void NetTables_Table::MergeFrom(const NetTables_Table& from) {
  GOOGLE_CHECK_NE(&from, this);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from.has_name()) {
      set_name(from.name());
    }
    if (from.has_maxentries()) {
      set_maxentries(from.maxentries());
    }
  }
  mutable_unknown_fields()->MergeFrom(from.unknown_fields());
}

void NetTables_Table::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void NetTables_Table::CopyFrom(const NetTables_Table& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool NetTables_Table::IsInitialized() const {
  if ((_has_bits_[0] & 0x00000003) != 0x00000003) return false;

  return true;
}

void NetTables_Table::Swap(NetTables_Table* other) {
  if (other != this) {
    std::swap(name_, other->name_);
    std::swap(maxentries_, other->maxentries_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.Swap(&other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::google::protobuf::Metadata NetTables_Table::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = NetTables_Table_descriptor_;
  metadata.reflection = NetTables_Table_reflection_;
  return metadata;
}


// -------------------------------------------------------------------

#ifndef _MSC_VER
const int NetTables::kTablesFieldNumber;
#endif  // !_MSC_VER

NetTables::NetTables()
  : ::google::protobuf::Message() {
  SharedCtor();
  // @@protoc_insertion_point(constructor:sv_cl_messages.NetTables)
}

void NetTables::InitAsDefaultInstance() {
}

NetTables::NetTables(const NetTables& from)
  : ::google::protobuf::Message() {
  SharedCtor();
  MergeFrom(from);
  // @@protoc_insertion_point(copy_constructor:sv_cl_messages.NetTables)
}

void NetTables::SharedCtor() {
  _cached_size_ = 0;
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

NetTables::~NetTables() {
  // @@protoc_insertion_point(destructor:sv_cl_messages.NetTables)
  SharedDtor();
}

void NetTables::SharedDtor() {
  if (this != default_instance_) {
  }
}

void NetTables::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* NetTables::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return NetTables_descriptor_;
}

const NetTables& NetTables::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_NetTables_2eproto();
  return *default_instance_;
}

NetTables* NetTables::default_instance_ = NULL;

NetTables* NetTables::New() const {
  return new NetTables;
}

void NetTables::Clear() {
  tables_.Clear();
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->Clear();
}

bool NetTables::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) goto failure
  ::google::protobuf::uint32 tag;
  // @@protoc_insertion_point(parse_start:sv_cl_messages.NetTables)
  for (;;) {
    ::std::pair< ::google::protobuf::uint32, bool> p = input->ReadTagWithCutoff(127);
    tag = p.first;
    if (!p.second) goto handle_unusual;
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // repeated .sv_cl_messages.NetTables.Table tables = 1;
      case 1: {
        if (tag == 10) {
         parse_tables:
          DO_(::google::protobuf::internal::WireFormatLite::ReadMessageNoVirtual(
                input, add_tables()));
        } else {
          goto handle_unusual;
        }
        if (input->ExpectTag(10)) goto parse_tables;
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
  // @@protoc_insertion_point(parse_success:sv_cl_messages.NetTables)
  return true;
failure:
  // @@protoc_insertion_point(parse_failure:sv_cl_messages.NetTables)
  return false;
#undef DO_
}

void NetTables::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // @@protoc_insertion_point(serialize_start:sv_cl_messages.NetTables)
  // repeated .sv_cl_messages.NetTables.Table tables = 1;
  for (int i = 0; i < this->tables_size(); i++) {
    ::google::protobuf::internal::WireFormatLite::WriteMessageMaybeToArray(
      1, this->tables(i), output);
  }

  if (!unknown_fields().empty()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
  // @@protoc_insertion_point(serialize_end:sv_cl_messages.NetTables)
}

::google::protobuf::uint8* NetTables::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // @@protoc_insertion_point(serialize_to_array_start:sv_cl_messages.NetTables)
  // repeated .sv_cl_messages.NetTables.Table tables = 1;
  for (int i = 0; i < this->tables_size(); i++) {
    target = ::google::protobuf::internal::WireFormatLite::
      WriteMessageNoVirtualToArray(
        1, this->tables(i), target);
  }

  if (!unknown_fields().empty()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        unknown_fields(), target);
  }
  // @@protoc_insertion_point(serialize_to_array_end:sv_cl_messages.NetTables)
  return target;
}

int NetTables::ByteSize() const {
  int total_size = 0;

  // repeated .sv_cl_messages.NetTables.Table tables = 1;
  total_size += 1 * this->tables_size();
  for (int i = 0; i < this->tables_size(); i++) {
    total_size +=
      ::google::protobuf::internal::WireFormatLite::MessageSizeNoVirtual(
        this->tables(i));
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

void NetTables::MergeFrom(const ::google::protobuf::Message& from) {
  GOOGLE_CHECK_NE(&from, this);
  const NetTables* source =
    ::google::protobuf::internal::dynamic_cast_if_available<const NetTables*>(
      &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void NetTables::MergeFrom(const NetTables& from) {
  GOOGLE_CHECK_NE(&from, this);
  tables_.MergeFrom(from.tables_);
  mutable_unknown_fields()->MergeFrom(from.unknown_fields());
}

void NetTables::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void NetTables::CopyFrom(const NetTables& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool NetTables::IsInitialized() const {

  if (!::google::protobuf::internal::AllAreInitialized(this->tables())) return false;
  return true;
}

void NetTables::Swap(NetTables* other) {
  if (other != this) {
    tables_.Swap(&other->tables_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.Swap(&other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::google::protobuf::Metadata NetTables::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = NetTables_descriptor_;
  metadata.reflection = NetTables_reflection_;
  return metadata;
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace sv_cl_messages

// @@protoc_insertion_point(global_scope)
