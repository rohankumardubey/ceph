// -*- mode:C++; tab-width:8; c-basic-offset:2; indent-tabs-mode:t -*-
// vim: ts=8 sw=2 smarttab ft=cpp

#ifndef CEPH_RGW_ROLE_H
#define CEPH_RGW_ROLE_H

#include <string>

#include "common/async/yield_context.h"

#include "common/ceph_json.h"
#include "common/ceph_context.h"

#include "rgw/rgw_rados.h"

namespace rgw { namespace sal {

class RGWRole
{
public:
  static const std::string role_name_oid_prefix;
  static const std::string role_oid_prefix;
  static const std::string role_path_oid_prefix;
  static const std::string role_arn_prefix;
  static constexpr int MAX_ROLE_NAME_LEN = 64;
  static constexpr int MAX_PATH_NAME_LEN = 512;
  static constexpr uint64_t SESSION_DURATION_MIN = 3600; // in seconds
  static constexpr uint64_t SESSION_DURATION_MAX = 43200; // in seconds
protected:

  std::string id;
  std::string name;
  std::string path;
  std::string arn;
  std::string creation_date;
  std::string trust_policy;
  std::map<std::string, std::string> perm_policy_map;
  std::string tenant;
  uint64_t max_session_duration;

public:
  virtual int store_info(const DoutPrefixProvider *dpp, bool exclusive, optional_yield y) = 0;
  virtual int store_name(const DoutPrefixProvider *dpp, bool exclusive, optional_yield y) = 0;
  virtual int store_path(const DoutPrefixProvider *dpp, bool exclusive, optional_yield y) = 0;
  virtual int read_id(const DoutPrefixProvider *dpp, const std::string& role_name, const std::string& tenant, std::string& role_id, optional_yield y) = 0;
  virtual int read_name(const DoutPrefixProvider *dpp, optional_yield y) = 0;
  virtual int read_info(const DoutPrefixProvider *dpp, optional_yield y) = 0;
  bool validate_input(const DoutPrefixProvider* dpp);
  void extract_name_tenant(const std::string& str);

  RGWRole(std::string name,
          std::string tenant,
          std::string path="",
          std::string trust_policy="",
          std::string max_session_duration_str="")
  : name(std::move(name)),
    path(std::move(path)),
    trust_policy(std::move(trust_policy)),
    tenant(std::move(tenant)) {
    if (this->path.empty())
      this->path = "/";
    extract_name_tenant(this->name);
    if (max_session_duration_str.empty()) {
      max_session_duration = SESSION_DURATION_MIN;
    } else {
      max_session_duration = std::stoull(max_session_duration_str);
    }
  }

  RGWRole(std::string id) : id(std::move(id)) {}

  virtual ~RGWRole() = default;

  void encode(bufferlist& bl) const {
    ENCODE_START(3, 1, bl);
    encode(id, bl);
    encode(name, bl);
    encode(path, bl);
    encode(arn, bl);
    encode(creation_date, bl);
    encode(trust_policy, bl);
    encode(perm_policy_map, bl);
    encode(tenant, bl);
    encode(max_session_duration, bl);
    ENCODE_FINISH(bl);
  }

  void decode(bufferlist::const_iterator& bl) {
    DECODE_START(2, bl);
    decode(id, bl);
    decode(name, bl);
    decode(path, bl);
    decode(arn, bl);
    decode(creation_date, bl);
    decode(trust_policy, bl);
    decode(perm_policy_map, bl);
    if (struct_v >= 2) {
      decode(tenant, bl);
    }
    if (struct_v >= 3) {
      decode(max_session_duration, bl);
    }
    DECODE_FINISH(bl);
  }

  const std::string& get_id() const { return id; }
  const std::string& get_name() const { return name; }
  const std::string& get_tenant() const { return tenant; }
  const std::string& get_path() const { return path; }
  const std::string& get_create_date() const { return creation_date; }
  const std::string& get_assume_role_policy() const { return trust_policy;}
  const uint64_t& get_max_session_duration() const { return max_session_duration; }

  void set_id(const std::string& id) { this->id = id; }

  virtual int create(const DoutPrefixProvider *dpp, bool exclusive, optional_yield y) = 0;
  virtual int delete_obj(const DoutPrefixProvider *dpp, optional_yield y) = 0;
  int get(const DoutPrefixProvider *dpp, optional_yield y);
  int get_by_id(const DoutPrefixProvider *dpp, optional_yield y);
  int update(const DoutPrefixProvider *dpp, optional_yield y);
  void update_trust_policy(std::string& trust_policy);
  void set_perm_policy(const std::string& policy_name, const std::string& perm_policy);
  std::vector<std::string> get_role_policy_names();
  int get_role_policy(const DoutPrefixProvider* dpp, const std::string& policy_name, std::string& perm_policy);
  int delete_policy(const DoutPrefixProvider* dpp, const std::string& policy_name);
  void dump(Formatter *f) const;
  void decode_json(JSONObj *obj);

  static const std::string& get_names_oid_prefix();
  static const std::string& get_info_oid_prefix();
  static const std::string& get_path_oid_prefix();
};
WRITE_CLASS_ENCODER(RGWRole)
} } // namespace rgw::sal
#endif /* CEPH_RGW_ROLE_H */
