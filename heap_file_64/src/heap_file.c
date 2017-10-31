#include "heap_file.h"
#include "bf.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

HP_ErrorCode HP_Init() {
  // write your code here
  return HP_OK;
}

HP_ErrorCode HP_CreateIndex(const char *filename) {
  // write your code here
    printf("create index in \n");
    int *file_desc;
    if(BF_CreateFile(filename) == BF_OK) {
        return HP_OK;
    }
    else {
        BF_PrintError(BF_CreateFile(filename));
        return HP_ERROR;
    }
}

HP_ErrorCode HP_OpenFile(const char *fileName, int *fileDesc){
  // write your code here
      printf("Open file in\n");
      if(BF_OpenFile(fileName,fileDesc) == BF_OK)
          return HP_OK;
      else
          return HP_ERROR;
    }

HP_ErrorCode HP_CloseFile(int fileDesc) {
  // write your code here
    printf("close file in\n");
    if(BF_CloseFile(fileDesc) == BF_OK)
        return HP_OK;
    else
        return HP_ERROR;
}

HP_ErrorCode HP_InsertEntry(int fileDesc, Record record) {
  // write your code here
    //printf("insert entry in\n");
    int block_num;
    int maxRecords = (BF_BLOCK_SIZE - 4)/ sizeof(record);
    BF_Block *block;
    BF_Block_Init(&block);
    BF_GetBlockCounter(fileDesc,&block_num);
    //printf("Block counter: %d" , block_num);
    if(block_num == 0)
        BF_AllocateBlock(fileDesc,block);

    BF_GetBlockCounter(fileDesc,&block_num);
    //printf("Block counter: %d" , block_num);

    if(BF_GetBlock(fileDesc,block_num-1,block) == BF_OK){
        //printf("In getblock!!!");
        char *blockData = BF_Block_GetData(block);
        int reccount;
        memcpy(&reccount,blockData, sizeof(int));
        if(reccount < maxRecords) {
            //printf("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@%d@%d@@@@@@@@@@@@",reccount,block_num);
            //printf("reccount<maxrecords!!!\n");
            //printf("%d,\"%s\",\"%s\",\"%s\"\n", record.id, record.name, record.surname, record.city);
            //char * strid = malloc(sizeof(int));
            //sprintf(strid,"%d",record.id);

            memcpy(blockData + sizeof(int) + (reccount* sizeof(record)), &record, sizeof(record));
            //memcpy(blockData + sizeof(int) + reccount*59, &record.id, sizeof(int));
            //memcpy(blockData + 2 * sizeof(int) + reccount*59, record.name, 15*sizeof(char));
            //memcpy(blockData + 2 * sizeof(int) + 15* sizeof(char) + reccount*59, record.surname, 20*sizeof(char));
            //memcpy(blockData + 2 * sizeof(int) + 35*sizeof(char) + reccount*59, record.city, 20*sizeof(char));

            int a;
            //for(int i=4+reccount* sizeof(record);i< 4+(reccount+1)* sizeof(record);i++) {
            //    printf("(%c) - ", blockData[i]);
            //}
            reccount++;
            memcpy(blockData, &reccount, sizeof(int));
            memcpy(&a,blockData, sizeof(int));
            BF_Block_SetDirty(block);
            BF_UnpinBlock(block);
            BF_Block_Destroy(&block);
        } else {
            BF_UnpinBlock(block);
            BF_AllocateBlock(fileDesc,block);
            char *block2Data = BF_Block_GetData(block);
            int c=1;
            memcpy(block2Data,&c, sizeof(int));
            memcpy(block2Data + sizeof(int), &record, sizeof(record));
            BF_Block_SetDirty(block);
            BF_UnpinBlock(block);
            BF_Block_Destroy(&block);
        }
        return HP_OK;
    } else {
        return HP_ERROR;
    }
}

HP_ErrorCode HP_PrintAllEntries(int fileDesc) {
  // write your code here
    printf("print all entries in\n");
    Record record;
    int blocksNum, maxRecords = (BF_BLOCK_SIZE - sizeof(int))/ sizeof(record);
    BF_GetBlockCounter(fileDesc,&blocksNum);
    int id=0, i, j, num, records_num = blocksNum * maxRecords;
    while(HP_GetEntry(fileDesc, id, &record) == HP_OK){
        id++;
        printf("%d,\"%s\",\"%s\",\"%s\"\n", record.id, record.name, record.surname, record.city);
    }
    return HP_OK;
}

HP_ErrorCode HP_GetEntry(int fileDesc, int rowId, Record *record) {
  // write your code here
    --rowId;
    int maxRecords = (BF_BLOCK_SIZE - sizeof(int))/ sizeof(*record);
    int rowBlock = (rowId/maxRecords);
    int i,j,num, blocksNum;
    int row = rowId%maxRecords;
    int endOfRow = (row * sizeof(*record)) + 4;
    BF_Block *block;
    BF_Block_Init(&block);
    BF_GetBlockCounter(fileDesc,&blocksNum);
    if(BF_GetBlock(fileDesc,rowBlock,block)==BF_OK){
        char *blockData = BF_Block_GetData(block);
        memcpy(record, blockData + endOfRow, 60);
        BF_UnpinBlock(block);
        BF_Block_Destroy(&block);
        return HP_OK;
    } else {
        return HP_ERROR;
    }

}
