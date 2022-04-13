//
// VniRegistry.h
//
// Copyright (C) 2022 Dialog Semiconductor
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of
// this software and associated documentation files (the "Software"), to deal in 
// the Software without restriction, including without limitation the rights to
// use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
// of the Software, and to permit persons to whom the Software is furnished to do
// so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

// Interface for the VniRegistry class.

#if !defined(AFX_VNIREGISTRY_H__5B495863_39DF_11D3_80B2_00105A202B29__INCLUDED_)
#define AFX_VNIREGISTRY_H__5B495863_39DF_11D3_80B2_00105A202B29__INCLUDED_

#include "VniDefs.h"
#include "VniUtil.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class VNIBASE_DECL VniRegistry  
{
public:
    VniRegistry();
    virtual ~VniRegistry();
        // Used to enumerate VNIs.  Index starts at 0.  Terminates (succefully)
        // with VNI_W_NO_MORE_VNIS.  The regPath may be in one of three forms:
        // a) Default.  regPath is NULL, and the VNIs are found in
        //    \HKEY_LOCAL_MACHINE\Software\LonWorks\VNI\Configuration.  Only the
        //    vniBaseName is returned.
        // b) Relative.  regPath is not NULL, and does not start with \.  The VNI
        //    is found at \HKEY_LOCAL_MACHINE\Software\LonWorks\<regPath>\VNIs.  The
        //    name of the VNI returned is <regPath>\<vniBaseName>
        // c) Absolute.  regPath starts with \.  VNIs are found in <regPath>\VNIs.  THe
        //    VNI name is <regPath>\<vniBaseName>, unless it could be expressed as a 
        //    relative path or a default path - in which case that name is used instead.
        //    ABSOLUTE NAMES NOT IMPLEMENTED.
    VniSts enumerateVni(const char *regPath, OUT VniStr &vniName, DWORD index) const;
        // Deprecated version, always looks in LONWORKS\VNI\Configuration
    VniSts enumerateVni(OUT VniStr &vniName, DWORD index) const 
        { return enumerateVni(NULL, vniName, index); };

        // Get a "short" VNI name, for display purposes.
    static void getVniShortName(const char *longName, OUT VniStr &shortName);
        // Translate the short VNI name back to a long VNI name.
    static void getVniLongName(const char *shortName, OUT VniStr &longName);

        // Get the name of the VNI device driver
    VniSts getVniDevice(IN const char *vniName, OUT VniStr &deviceName);
         // Set the name of the VNI device driver
    VniSts setVniDevice(IN const char *vniName, IN const char *deviceName, boolean export = TRUE);
       // Get the path of the VNI directory
    VniSts getVniDirectoryPath(IN const char *vniName, OUT VniStr &directoryPath);
    BOOLEAN vniExists(IN const char *vniName);

    VniSts getVniProgramId(IN const char *vniName, OUT VniProgramId *programId);
    VniSts getVniMaxDomains(IN const char *vniName, OUT int &maxDomains);
    VniSts getVniMaxAddresses(IN const char *vniName, OUT int &maxAddresses);
    VniSts getVniMaxStaticNvs(IN const char *vniName, OUT int &maxStaticNvs);
    VniSts getVniMaxDynamicNvs(IN const char *vniName, OUT int &maxDynamicNvs);
    VniSts getVniMaxAliases(IN const char *vniName, OUT int &maxAliases);
    VniSts getVniNodeSelfDoc(IN const char *vniName, OUT VniStr &nodeSelfDoc);
    VniSts getVniUniqueId(IN const char *vniName, OUT VniUniqueId *uniqueId);
    VniSts getVniNumMonitorNvEntries(IN const char *vniName, OUT int &numMonitorNvEntries);
    VniSts getVniNumMonitorPointEntries(IN const char *vniName, OUT int &numMonitorPointEntries);
    VniSts getVniNumMonitorSetEntries(IN const char *vniName, OUT int &numMonitorSetEntries);
    VniSts getVniNumMessageTags(IN const char *vniName, OUT int &numMessageTags);
    VniSts getVniPollingLimits(IN const char *vniName, 
                               OUT int &numReservedMsgs, 
                               OUT int &pollingLimitThreshold,
                               OUT int &initialPollLimit,
                               OUT int &minimumPollLimit);
    VniSts setVniPollingLimits(IN const char *vniName, 
                               IN int numReservedMsgs, 
                               IN int pollingLimitThreshold,
                               IN int initialPollLimit,
                               IN int minimumPollLimit);

        // Get and set global VNI parameters (do not effect external interface)
    VniSts getVniMaxPrivateNvs(OUT int &maxPrivatNvs);
    VniSts setVniMaxPrivateNvs(OUT int maxPrivatNvs);
    VniSts getVniReceiveTx(OUT int &recieveTx);
    VniSts setVniReceiveTx(IN int recieveTx);
    VniSts getVniTransmitTx(OUT int &transmitTx);
    VniSts setVniTransmitTx(IN int transmitTx);
    VniSts getVniTransactionIdLifeTime(OUT int &transactionIdLifeTime);
    VniSts setVniTransactionIdLifeTime(IN int transactionIdLifeTime);
    VniSts getVniMessageEventMaximum(OUT int &count);
    VniSts setVniMessageEventMaximum(IN int count);
    VniSts getVniMessageOutMaximum(OUT int &count, OUT int &countPri);
    VniSts setVniMessageOutMaximum(IN int count, IN int countPri);
    VniSts getVniServerCmdThreadStackSize(OUT int &stackSize);
    VniSts setVniServerCmdThreadStackSize(IN int stackSize);
    VniSts getVniDefaultPollingLimits(OUT int &numReservedMsgs, OUT int &pollingLimitThreshold,
                                      OUT int &initialPollLimit, OUT int &minimumPollLimit);
    VniSts setVniDefaultPollingLimits(IN int numReservedMsgs, IN int pollingLimitThreshold,
                                      IN int initialPollLimit, IN int minimumPollLimit);

        // Get and set device parameters
    static VniSts getDeviceTimeouts(const char *deviceName, int &openTimeout, int &responseTimeout, int &openRetries);
    static VniSts getDeviceConfiguredMipLayer(const char *deviceName, int &mipLayer, int &xcvrId, boolean &bIsNsaMip);
    static VniSts getDeviceXcvrId(const char *deviceName, int &xcvrId);
    static VniSts getDeviceConfiguredBufferSizes(const char *deviceName, int &maxSicbData);
    static VniSts getDeviceConfiguredAdvancedTxTickler(const char *deviceName, boolean &advancedTxTickler);
    static VniSts getDeviceConfiguredNmVersion(const char *deviceName, int &nmVersion, int &nmCapabilities);
	static VniSts getDeviceTurnaround(const char *deviceName, int &turnAround);
    static VniSts getDeviceSupportsEncryption(const char *deviceName, boolean &supportsEncryption);

    static VniSts setDeviceTimeouts(const char *deviceName, int openTimeout, int responseTimeout, int openRetries);
    static VniSts setDeviceTimeoutDefaults(int openTimeout, int responseTimeout, int openRetries);
    static VniSts setDeviceConfiguredMipLayer(const char *deviceName, int mipLayer, int xcvrId, boolean isNsaMip);
    static VniSts setDeviceXcvrId(const char *deviceName, int xcvrId);
    static VniSts setDeviceConfiguredBufferSizes(const char *deviceName, int maxSicbData);
    static VniSts setDeviceConfiguredAdvancedTxTickler(const char *deviceName, boolean advancedTxTickler);
    static VniSts setDeviceConfiguredNmVersion(const char *deviceName, int nmVersion, int nmCapabilities);
	static VniSts setDeviceTurnaround(const char *deviceName, int turnAround);
    static VniSts setDeviceSupportsEncryption(const char *deviceName, boolean supportsEncryption);

    static VniSts deleteDeviceTimeouts(const char *deviceName);
    static VniSts deleteDeviceTimeoutDefaults();
    static VniSts deleteDeviceConfiguredMipLayer(const char *deviceName);
    static VniSts deleteDeviceXcvrId(const char *deviceName);
    static VniSts deleteDeviceConfiguredBufferSizes(const char *deviceName);
    static VniSts deleteDeviceConfiguredAdvancedTxTickler(const char *deviceName);
    static VniSts deleteDeviceConfiguredNmVersion(const char *deviceName);
    static VniSts deleteDeviceSupportsEncryption(const char *deviceName);

    // Locking and version numbers
    VniSts lockVniDir(boolean write);

    void unlockVniDir();

    VniSts getDirectoryVersionNumber(VniDirVersionNum &vniDirVersion);

    VniSts getServerIpcSpace(int &ipcSpace);
    VniSts setServerIpcSpace(int ipcSpace);

    VniSts setVniUniqueId(IN const char *vniName, OUT VniUniqueId *uniqueId, boolean export = TRUE);


protected:
        /* Create a new VNI. */
    VniSts createVni(IN const char *vniName, 
                     IN const char *pVniDirectory,
                     IN const char *pNetworkInterface,
                     IN const VniProgramId  programId,
                     IN int                 maxDomains,
                     IN int                 maxAddresses,
                     IN int                 maxStaticNvs,
                     IN int                 maxDynamicNvs,
                     IN int                 maxAliases,
                     IN const char*         nodeSelfDoc,
                     IN VniUniqueId         uniqueId,
                     IN int                 numMonitorNvEntries,
                     IN int                 numMonitorPointEntries,
                     IN int                 numMonitorSetEntries,
                     IN int                 numMessageTags);

    VniSts importVni(IN const char *vniName, IN const char *pVniDirectory);
    VniSts exportVni(IN const char *vniName);

    VniSts deleteVni(IN const char *vniName);

private:
    VniSts getVniEntry(IN const char *vniName,
                       int            dataItem,
                       void          *pDataItemValue,
                       DWORD         *pDataItemLen);

    VniSts getVniEntry(IN const char *vniName,
                       int            dataItem,
                       VniStr        &vniStr);

    VniSts getVniHexStringEntry(IN const char *vniName,
                                int            dataItem,
                                byte          *pDataItemValue,
                                DWORD          dataItemLen);

    VniSts setVniEntry(IN const char *vniName,
                       int            dataItem,
                       const void    *pDataItemValue,
                       DWORD          dataItemLen,
                       boolean        export = TRUE);

    VniSts setVniEntry(IN const char *vniName,
                       int            dataItem,
                       const char    *strValue,
                       boolean        export = TRUE);

    static VniSts getVniGlobalEntry(int             dataItem,
                             void           *pDataItemValue,
                             DWORD          *pDataItemLen,
                             boolean        *pUsingDefault = NULL);

    static VniSts setVniGlobalEntry(int              dataItem,
                             const void      *pDataItemValue,
                             DWORD            dataItemLen);
    
    static VniSts deleteVniGlobalEntry(int dataItem);

    VniSts setVniHexStringEntry(IN const char *vniName,
                                int            dataItem,
                                byte          *pDataItemValue,
                                DWORD          dataItemLen,
                                boolean        export = TRUE);

    VniSts getRegPath(IN const char *vniName, HKEY &rootKey, 
                      char *primaryKey, char *entryName);
    VniSts getRootPath(IN const char *vniName, HKEY &rootKey, 
                       char *rootPath, char *entryName);
    VniSts getVniRootDir(IN const char *vniName, char *pVniRootDir, DWORD rootDirLen);
    static VniSts getDeviceReg(const char *deviceName, int dataItem, int &value);
    static VniSts setDeviceReg(const char *deviceName, int dataItem, int value);
    static VniSts deleteDeviceReg(const char *deviceName, int dataItem);
    static void getDeviceRegPath(const char *deviceName, VniStr& path, VniStr& name);
    static boolean isXdriver(const char *deviceName);


    int              m_lockCount;
    BOOLEAN          m_versionAvailable;
    BOOLEAN          m_writeLocked;
    BOOLEAN          m_dirOpen;
    HKEY             m_dirKey;
    VniDirVersionNum m_versionNum;
};

#endif // !defined(AFX_VNIREGISTRY_H__5B495863_39DF_11D3_80B2_00105A202B29__INCLUDED_)
