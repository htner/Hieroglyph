/*-------------------------------------------------------------------------
 *
 * modify_state.hpp
 *		  FDW routines for parquet_s3_fdw
 *
 * Portions Copyright (c) 2022, TOSHIBA CORPORATION
 *
 * IDENTIFICATION
 *		  contrib/parquet_s3_fdw/src/modify_state.hpp
 *
 *-------------------------------------------------------------------------
 */
#pragma once

#include <list>
#include <set>
#include <vector>

#include "backend/sdb/common/pg_export.hpp"
#include "backend/access/parquet/parquet_writer.hpp"

class ParquetS3WriterState {
private:
  /* list parquet reader of target files */
  std::shared_ptr<ParquetWriter> inserter_;
  std::map<uint64_t, std::shared_ptr<ParquetWriter>> updates_;
  std::list<std::shared_ptr<ParquetWriter>> uploads_;
  /* memory context of reader */
  MemoryContext cxt;
  /* target directory name */
  std::string dirname;
  /* S3 system client */
  Aws::S3::S3Client *s3_client;
  /* foreign table desc */
  TupleDesc tuple_desc;

  /* list attnum of needed modify attributes */
  std::set<int> target_attrs;

  /* parquet reader option */
  bool use_threads;
  bool use_mmap;
  /* foreign table name */
  char *rel_name;
  Oid rel_id;

 public:
  ParquetS3WriterState(MemoryContext reader_cxt, const char *dirname,
                       Aws::S3::S3Client *s3_client, TupleDesc tuple_desc,
                       std::set<int> target_attrs, bool use_threads,
                       bool use_mmap);
  ~ParquetS3WriterState();

  /* create reader for `filename` and add to list file */
  //void AddFile(uint64_t blockid, const char *filename);
  /* create new file and its temporary cache data */
  std::shared_ptr<ParquetWriter> NewInserter(const char *filename);

  // void AddUpdateFile(uint64_t fileid, const char *filename);
  /* execute insert `*slot` to cache data */
  bool ExecInsert(TupleTableSlot *slot);
  /* execute update */
  bool ExecUpdate(TupleTableSlot *slot, TupleTableSlot *planSlot);
  /* execute delete */
  bool ExecDelete(ItemPointer tic);
  /* upload modified parquet file to storage system (local/S3) */
  void Upload();
  void CommitUpload();
  //                 std::list<lake::LakeFile> delete_files);
  /* true if s3_client is set */
  bool HasS3Client();

  /* create schema for new file */
  std::shared_ptr<arrow::Schema> CreateNewFileSchema();

  void SetRel(char *name, Oid id);

  // FIXME
  std::map<uint64_t, std::shared_ptr<ParquetWriter>>& Updates() {
    return updates_;
  }
};

ParquetS3WriterState *create_parquet_modify_state(
    MemoryContext reader_cxt, const char *dirname, Aws::S3::S3Client *s3_client,
    TupleDesc tuple_desc, std::set<int> target_attrs, bool use_threads,
    bool use_mmap);

