// Copyright 2021 Google LLC

#include "firestore/src/main/write_batch_main.h"

#include <utility>

#include "Firestore/core/src/core/user_data.h"
#include "Firestore/core/src/model/field_path.h"
#include "firestore/src/main/converter_main.h"
#include "firestore/src/main/document_reference_main.h"
#include "firestore/src/main/field_value_main.h"
#include "firestore/src/main/listener_main.h"
#include "firestore/src/main/set_options_main.h"
#include "firestore/src/main/util_main.h"

namespace firebase {
namespace firestore {

using core::ParsedSetData;
using core::ParsedUpdateData;

WriteBatchInternal::WriteBatchInternal(api::WriteBatch&& batch)
    : batch_{std::move(batch)},
      promise_factory_{PromiseFactory<AsyncApis>::Create(this)},
      user_data_converter_{&firestore_internal()->database_id()} {}

Firestore* WriteBatchInternal::firestore() { return GetFirestore(&batch_); }

FirestoreInternal* WriteBatchInternal::firestore_internal() {
  return GetFirestoreInternal(&batch_);
}

void WriteBatchInternal::Set(const DocumentReference& document,
                             const MapFieldValue& data,
                             const SetOptions& options) {
  ParsedSetData parsed = user_data_converter_.ParseSetData(data, options);
  batch_.SetData(GetCoreApi(document), std::move(parsed));
}

void WriteBatchInternal::Update(const DocumentReference& document,
                                const MapFieldValue& data) {
  batch_.UpdateData(GetCoreApi(document),
                    user_data_converter_.ParseUpdateData(data));
}

void WriteBatchInternal::Update(const DocumentReference& document,
                                const MapFieldPathValue& data) {
  batch_.UpdateData(GetCoreApi(document),
                    user_data_converter_.ParseUpdateData(data));
}

void WriteBatchInternal::Delete(const DocumentReference& document) {
  batch_.DeleteData(GetCoreApi(document));
}

Future<void> WriteBatchInternal::Commit() {
  auto promise = promise_factory_.CreatePromise<void>(AsyncApis::kCommit);
  auto callback = StatusCallbackWithPromise(promise);
  batch_.Commit(std::move(callback));
  return promise.future();
}

}  // namespace firestore
}  // namespace firebase
