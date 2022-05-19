/*
 * @(#)levelpack.c
 *
 * Copyright 2001-2002, Aaron Ardiri     (mailto:aaron@ardiri.com)
 *                      Charles Kerchner (mailto:chip@ardiri.com)
 * All rights reserved.
 *
 * This file was generated as part of the  "lemmings" program developed
 * for the Palm Computing Platform designed by Palm:
 *
 *   http://www.palm.com/
 *
 * The contents of this file is confidential and proprietrary in nature
 * ("Confidential Information"). Redistribution or modification without
 * prior consent of the original author is prohibited.
 */

#include "palm.h"

typedef struct
{
  PreferencesType *prefs;

  UInt8            type;             // which type of level are we dealing with?
  Char             strLevelPack[32]; // the level pack name

  union
  {
    struct
    {
      UInt16        card;
      LocalID       dbID;
      DmOpenRef     dbRef;
    } ram;                           // ram based level pack

    struct
    {
      FileRef       fileRef;
    } vfs;                           // vfs based level pack

  } db;

} Globals;

static Globals globals;

/**
 * Initialize the level pack routines.
 *
 * @param the global preferences structure.
 */
void
LevelPackInitialize(PreferencesType *prefs)
{
  MemSet(&globals, sizeof(Globals), 0);
  globals.prefs = prefs;

  // default to the standard level pack [do we need to do this]
  globals.type = PACK_INTERNAL;
  MemSet(globals.strLevelPack, 32, 0);
}

/**
 * Check if a level pack exists (it may have been removed)?
 *
 * @param packType     the type of level pack
 * @param strLevelPack the name of the level pack (could be filename)
 * @returns true if level pack exists, false otherwise
 */
Boolean
LevelPackExists(UInt8 packType, UInt8 *strLevelPack)
{
  Boolean result = false;
  Char    fullPath[256];
  UInt16  volRef;
  UInt32  volIterator, version;
  Err     err;

  switch (packType)
  {
    case PACK_INTERNAL:
         result = true;   // doh! :)
         break;

    case PACK_RAM:
         result = (DmFindDatabase(0, strLevelPack) != NULL);
         break;

    case PACK_VFS:
         err = FtrGet(sysFileCVFSMgr, vfsFtrIDVersion, &version);
         if ((err == errNone) && (!result))
         {
           // generate the filename
           StrPrintF(fullPath, "%s%s", VFS_DIRECTORY, strLevelPack);

           // loop through all the mounted volumes.
           volIterator = vfsIteratorStart;
           while ((volIterator != vfsIteratorStop) && !result)
           {
             err = VFSVolumeEnumerate(&volRef, &volIterator);
             if (err == errNone)
             {
               // look for the database on this volume.
               err = VFSFileOpen(volRef, fullPath, vfsModeRead, &globals.db.vfs.fileRef);

	             // did we find the resource?
       	       if (err == errNone)
       	       {
       	       	 result = true;
       	       	 VFSFileClose(globals.db.vfs.fileRef);
       	       }
             }
           }
         }
         break;

#ifdef MDM_DISTRIBUTION
    case PACK_VFS_MDM:
         {
           UInt8  MDM_key[dlkUserNameBufSize] = { };
           UInt32 MDM_key_length = 0;

           // do system level check that key file exists/card not read/write
           err = MDM_GetKey(MDM_key, &MDM_key_length);
           if (err != errNone) goto MDM_CHECK_ABORT;
         }

         err = FtrGet(sysFileCVFSMgr, vfsFtrIDVersion, &version);
         if ((err == errNone) && (!result))
         {
           // generate the filename
           StrPrintF(fullPath, "%s%s", VFS_MDM_DIRECTORY, strLevelPack);

           // loop through all the mounted volumes.
           volIterator = vfsIteratorStart;
           while ((volIterator != vfsIteratorStop) && !result)
           {
             err = VFSVolumeEnumerate(&volRef, &volIterator);
             if (err == errNone)
             {
               // look for the database on this volume.
               err = VFSFileOpen(volRef, fullPath, vfsModeRead, &globals.db.vfs.fileRef);

	       // did we find the resource?
       	       if (err == errNone)
       	       {
       	       	 result = true;
       	       	 VFSFileClose(globals.db.vfs.fileRef);
       	       }
             }
           }
         }

MDM_CHECK_ABORT:

         break;
#endif

    default:
         break;
  }

  return result;
}

/**
 * Open level pack.
 *
 * @param packType     the type of level pack
 * @param strLevelPack the name of the level pack (could be filename)
 */
void
LevelPackOpen(UInt8 packType, UInt8 *strLevelPack)
{
  Char    fullPath[256];
  UInt16  volRef;
  UInt32  volIterator, version;
  Err     err;
  Boolean done = false;

  switch (packType)
  {
    case PACK_INTERNAL:
         SysCurAppDatabase(&globals.db.ram.card, &globals.db.ram.dbID);
         globals.db.ram.dbRef =
           DmOpenDatabase(globals.db.ram.card, globals.db.ram.dbID, dmModeReadOnly);
         break;

    case PACK_RAM:
         globals.db.ram.card = 0;
         globals.db.ram.dbID = DmFindDatabase(globals.db.ram.card, strLevelPack);
         globals.db.ram.dbRef =
           DmOpenDatabase(globals.db.ram.card, globals.db.ram.dbID, dmModeReadOnly);
         break;

    case PACK_VFS:
         err = FtrGet(sysFileCVFSMgr, vfsFtrIDVersion, &version);
         if ((err == errNone) && (!done))
         {
           // generate the filename
           StrPrintF(fullPath, "%s%s", VFS_DIRECTORY, strLevelPack);

           // loop through all the mounted volumes.
           volIterator = vfsIteratorStart;
           while ((volIterator != vfsIteratorStop) && !done)
           {
             err = VFSVolumeEnumerate(&volRef, &volIterator);
             if (err == errNone)
             {
               // look for the database on this volume.
               err = VFSFileOpen(volRef, fullPath, vfsModeRead, &globals.db.vfs.fileRef);
               done = (err == errNone);
             }
           }
         }
         break;

#ifdef MDM_DISTRIBUTION
    case PACK_VFS_MDM:
         err = FtrGet(sysFileCVFSMgr, vfsFtrIDVersion, &version);
         if ((err == errNone) && (!done))
         {
           // generate the filename
           StrPrintF(fullPath, "%s%s", VFS_MDM_DIRECTORY, strLevelPack);

           // loop through all the mounted volumes.
           volIterator = vfsIteratorStart;
           while ((volIterator != vfsIteratorStop) && !done)
           {
             err = VFSVolumeEnumerate(&volRef, &volIterator);
             if (err == errNone)
             {
               // look for the database on this volume.
               err = VFSFileOpen(volRef, fullPath, vfsModeRead, &globals.db.vfs.fileRef);
               done = (err == errNone);
             }
           }
         }
         break;
#endif

    default:
         break;
  }

  // set the level pack type
  globals.type = packType;
}

/**
 * Get a resource from the level pack.
 *
 * @param type resource type
 * @param id   the resource id
 * @return a handle to the resource.
 */
MemHandle
LevelPackGetResource(UInt32 type, UInt16 id)
{
  MemHandle memHandle = NULL;

  switch (globals.type)
  {
    case PACK_INTERNAL:
    case PACK_RAM:
         memHandle = DmGetResource(type, id);
         break;

    case PACK_VFS:
#ifdef MDM_DISTRIBUTION
    case PACK_VFS_MDM:
#endif
         VFSFileDBGetResource(globals.db.vfs.fileRef, type, id, &memHandle);
         break;

    default:
         break;
  }

  return memHandle;
}

/**
 * Release a resource.
 *
 * @param memHandle handle to release.
 */
void
LevelPackReleaseResource(MemHandle memHandle)
{
  switch (globals.type)
  {
    case PACK_INTERNAL:
    case PACK_RAM:
         DmReleaseResource(memHandle);
         break;

    case PACK_VFS:
#ifdef MDM_DISTRIBUTION
    case PACK_VFS_MDM:
#endif
         MemHandleFree(memHandle);
         break;

    default:
         break;
  }
}

/**
 * Close level pack.
 */
void
LevelPackClose()
{
  switch (globals.type)
  {
    case PACK_INTERNAL:
    case PACK_RAM:
         DmCloseDatabase(globals.db.ram.dbRef);
         break;

    case PACK_VFS:
#ifdef MDM_DISTRIBUTION
    case PACK_VFS_MDM:
#endif
         VFSFileClose(globals.db.vfs.fileRef);
         break;

    default:
         break;
  }
}

/**
 * Terminate the level pack routines.
 */
void
LevelPackTerminate()
{
}
