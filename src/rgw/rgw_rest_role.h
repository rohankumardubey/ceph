// -*- mode:C++; tab-width:8; c-basic-offset:2; indent-tabs-mode:t -*-
// vim: ts=8 sw=2 smarttab ft=cpp

#pragma once

#include "common/async/yield_context.h"

#include "rgw_role.h"
#include "rgw_rest.h"

class RGWRestRole : public RGWRESTOp {
protected:
  std::string role_name;
  std::string role_path;
  std::string trust_policy;
  std::string policy_name;
  std::string perm_policy;
  std::string path_prefix;
  std::string max_session_duration;
  std::unique_ptr<rgw::sal::RGWRole> _role;
public:
  int verify_permission(optional_yield y) override;
  void send_response() override;
  virtual uint64_t get_op() = 0;
};

class RGWRoleRead : public RGWRestRole {
public:
  RGWRoleRead() = default;
  int check_caps(const RGWUserCaps& caps) override;
};

class RGWRoleWrite : public RGWRestRole {
public:
  RGWRoleWrite() = default;
  int check_caps(const RGWUserCaps& caps) override;
};

class RGWCreateRole : public RGWRoleWrite {
public:
  RGWCreateRole() = default;
  int verify_permission(optional_yield y) override;
  void execute(optional_yield y) override;
  int get_params();
  const char* name() const override { return "create_role"; }
  RGWOpType get_type() override { return RGW_OP_CREATE_ROLE; }
  uint64_t get_op() override { return rgw::IAM::iamCreateRole; }
};

class RGWDeleteRole : public RGWRoleWrite {
public:
  RGWDeleteRole() = default;
  void execute(optional_yield y) override;
  int get_params();
  const char* name() const override { return "delete_role"; }
  RGWOpType get_type() override { return RGW_OP_DELETE_ROLE; }
  uint64_t get_op() override { return rgw::IAM::iamDeleteRole; }
};

class RGWGetRole : public RGWRoleRead {
  int _verify_permission(const rgw::sal::RGWRole* role);
public:
  RGWGetRole() = default;
  int verify_permission(optional_yield y) override;
  void execute(optional_yield y) override;
  int get_params();
  const char* name() const override { return "get_role"; }
  RGWOpType get_type() override { return RGW_OP_GET_ROLE; }
  uint64_t get_op() override { return rgw::IAM::iamGetRole; }
};

class RGWModifyRole : public RGWRoleWrite {
public:
  RGWModifyRole() = default;
  void execute(optional_yield y) override;
  int get_params();
  const char* name() const override { return "modify_role"; }
  RGWOpType get_type() override { return RGW_OP_MODIFY_ROLE; }
  uint64_t get_op() override { return rgw::IAM::iamModifyRole; }
};

class RGWListRoles : public RGWRoleRead {
public:
  RGWListRoles() = default;
  int verify_permission(optional_yield y) override;
  void execute(optional_yield y) override;
  int get_params();
  const char* name() const override { return "list_roles"; }
  RGWOpType get_type() override { return RGW_OP_LIST_ROLES; }
  uint64_t get_op() override { return rgw::IAM::iamListRoles; }
};

class RGWPutRolePolicy : public RGWRoleWrite {
public:
  RGWPutRolePolicy() = default;
  void execute(optional_yield y) override;
  int get_params();
  const char* name() const override { return "put_role_policy"; }
  RGWOpType get_type() override { return RGW_OP_PUT_ROLE_POLICY; }
  uint64_t get_op() override { return rgw::IAM::iamPutRolePolicy; }
};

class RGWGetRolePolicy : public RGWRoleRead {
public:
  RGWGetRolePolicy() = default;
  void execute(optional_yield y) override;
  int get_params();
  const char* name() const override { return "get_role_policy"; }
  RGWOpType get_type() override { return RGW_OP_GET_ROLE_POLICY; }
  uint64_t get_op() override { return rgw::IAM::iamGetRolePolicy; }
};

class RGWListRolePolicies : public RGWRoleRead {
public:
  RGWListRolePolicies() = default;
  void execute(optional_yield y) override;
  int get_params();
  const char* name() const override { return "list_role_policies"; }
  RGWOpType get_type() override { return RGW_OP_LIST_ROLE_POLICIES; }
  uint64_t get_op() override { return rgw::IAM::iamListRolePolicies; }
};

class RGWDeleteRolePolicy : public RGWRoleWrite {
public:
  RGWDeleteRolePolicy() = default;
  void execute(optional_yield y) override;
  int get_params();
  const char* name() const override { return "delete_role_policy"; }
  RGWOpType get_type() override { return RGW_OP_DELETE_ROLE_POLICY; }
  uint64_t get_op() override { return rgw::IAM::iamDeleteRolePolicy; }
};
