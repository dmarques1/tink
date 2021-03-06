// Copyright 2018 Google Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
///////////////////////////////////////////////////////////////////////////////

#ifndef TINK_DAEAD_DETERMINISTIC_AEAD_SET_WRAPPER_H_
#define TINK_DAEAD_DETERMINISTIC_AEAD_SET_WRAPPER_H_

#include "absl/strings/string_view.h"
#include "tink/deterministic_aead.h"
#include "tink/primitive_set.h"
#include "tink/util/statusor.h"
#include "proto/tink.pb.h"

namespace crypto {
namespace tink {

// Wraps a set of DeterministicAead-instances that correspond to a keyset,
// and combines them into a single DeterministicAead-primitive, that uses
// the provided instances, depending on the context:
//   * DeterministicAead::EncryptDeterministically(...) uses the primary
//     instance from the set
//   * DeterministicAead::DecryptDeterministically(...) uses the instance
//     that matches the ciphertext prefix.
class DeterministicAeadSetWrapper : public DeterministicAead {
 public:
  // Returns a DeterministicAead-primitive that uses Daead-instances provided
  // in 'daead_set', which must be non-NULL and must contain a primary instance.
  static crypto::tink::util::StatusOr<std::unique_ptr<DeterministicAead>>
  NewDeterministicAead(
      std::unique_ptr<PrimitiveSet<DeterministicAead>> daead_set);

  crypto::tink::util::StatusOr<std::string> EncryptDeterministically(
      absl::string_view plaintext,
      absl::string_view associated_data) const override;

  crypto::tink::util::StatusOr<std::string> DecryptDeterministically(
      absl::string_view ciphertext,
      absl::string_view associated_data) const override;

  virtual ~DeterministicAeadSetWrapper() {}

 private:
  std::unique_ptr<PrimitiveSet<DeterministicAead>> daead_set_;

  DeterministicAeadSetWrapper(
      std::unique_ptr<PrimitiveSet<DeterministicAead>> daead_set)
      : daead_set_(std::move(daead_set)) {}
};

}  // namespace tink
}  // namespace crypto

#endif  // TINK_DAEAD_DETERMINISTIC_AEAD_SET_WRAPPER_H_
