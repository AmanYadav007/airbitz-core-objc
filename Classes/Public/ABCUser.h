//
//  ABCUser.h
//  Airbitz
//

#import "ABCWallet.h"
#import "ABCTransaction.h"
#import "ABCConditionCode.h"
#import "ABCSpend.h"
#import "ABCRequest.h"
#import "ABCSettings.h"
#import "AirbitzCore.h"
#import "ABCUser.h"

#define CONFIRMED_CONFIRMATION_COUNT 6
#define PIN_REQUIRED_PERIOD_SECONDS     120

@class ABCSpend;
@class AirbitzCore;
@class ABCSettings;
@class ABCRequest;
@class ABCTransaction;

@interface BitidSignature : NSObject
@property (nonatomic, strong) NSString *address;
@property (nonatomic, strong) NSString *signature;
@end

@protocol ABCUserDelegate <NSObject>

@optional

- (void) abcUserRemotePasswordChange;
- (void) abcUserLoggedOut;
- (void) abcUserDataSyncUpdate;
- (void) abcUserWalletsLoading;
- (void) abcUserWalletsLoaded;
- (void) abcUserWalletsChanged;
- (void) abcUserOTPRequired;
- (void) abcUserOTPSkew;
- (void) abcUserExchangeRateChanged;
- (void) abcUserIncomingBitcoin:(NSString *)walletUUID txid:(NSString *)txid;

@end

@interface ABCUser : NSObject

/// @name AirbitzCore read/write public object variables

/// Delegate object to handle delegate callbacks
@property (assign)            id<ABCUserDelegate>    delegate;

/// ABC settings that can be set or viewed by app or ABC. Use method [ABCSettings loadSettings]
/// to make sure they are loaded and [ABCSettings saveSettings] to ensure modified settings are latched
@property (nonatomic, strong) ABCSettings               *settings;

/// @name AirbitzCore read-only public object variables

/// Array of Wallet objects currently loaded into account. This array is read-only and app should only
/// access the array while in the main queue.
@property (nonatomic, strong) NSMutableArray            *arrayWallets;

/// Array of archived Wallet objects currently loaded into account. This array is read-only and app should only
/// access the array while in the main queue.
@property (nonatomic, strong) NSMutableArray            *arrayArchivedWallets;

/// Array of NSString * wallet names. This array is read-only and app should only
/// access the array while in the main queue.
@property (nonatomic, strong) NSMutableArray            *arrayWalletNames;

@property (nonatomic, strong) ABCWallet                 *currentWallet;
@property (nonatomic, strong) NSArray                   *arrayCategories;
@property (nonatomic)         int                       currentWalletID;
@property (nonatomic)         BOOL                      bAllWalletsLoaded;
@property (nonatomic)         int                       numWalletsLoaded;
@property (nonatomic)         int                       numTotalWallets;
@property (nonatomic)         int                       numCategories;

@property (nonatomic, copy)     NSString                *name;
@property (nonatomic, copy)     NSString                *password;

@property (nonatomic, strong)   AirbitzCore             *abc;



- (id)init:(AirbitzCore *)airbitzCore;
- (void)free;
- (void)startQueues;
- (void)stopQueues;

- (void)clearSyncQueue;
- (void)clearTxSearchQueue;

- (int)dataOperationCount;

// New methods
- (void)rotateWalletServer:(NSString *)walletUUID refreshData:(BOOL)bData notify:(void(^)(void))cb;
- (void)reorderWallets: (NSIndexPath *)sourceIndexPath toIndexPath:(NSIndexPath *)destinationIndexPath;
- (void)makeCurrentWallet:(ABCWallet *)wallet;
- (void)makeCurrentWalletWithIndex:(NSIndexPath *)indexPath;
- (void)makeCurrentWalletWithUUID:(NSString *)strUUID;
- (ABCWallet *)selectWalletWithUUID:(NSString *)strUUID;
- (long) saveLogoutDate;
- (void)addCategory:(NSString *)strCategory;
- (void)loadCategories;
- (void)saveCategories:(NSMutableArray *)saveArrayCategories;
- (BOOL)accountExistsLocal:(NSString *)username;
- (BOOL) isLoggedIn;



- (ABCWallet *)getWallet: (NSString *)walletUUID;

- (bool)setWalletAttributes: (ABCWallet *) wallet;

- (NSMutableArray *)searchTransactionsIn: (ABCWallet *) wallet query:(NSString *)term addTo:(NSMutableArray *) arrayTransactions;
- (void)storeTransaction:(ABCTransaction *)transaction;

- (int) currencyDecimalPlaces;
- (int) maxDecimalPlaces;
- (int64_t) cleanNumString:(NSString *) value;
- (NSString *)formatCurrency:(double) currency withCurrencyNum:(int)currencyNum;
- (NSString *)formatCurrency:(double) currency withCurrencyNum:(int)currencyNum withSymbol:(bool)symbol;
- (NSString *)formatSatoshi:(int64_t) bitcoin;
- (NSString *)formatSatoshi:(int64_t) bitcoin withSymbol:(bool) symbol;
- (NSString *)formatSatoshi:(int64_t) bitcoin withSymbol:(bool) symbol cropDecimals:(int) decimals;
- (NSString *)formatSatoshi:(int64_t) bitcoin withSymbol:(bool) symbol forceDecimals:(int) forcedecimals;
- (NSString *)formatSatoshi:(int64_t) bitcoin withSymbol:(bool) symbol cropDecimals:(int) decimals forceDecimals:(int) forcedecimals;
- (int64_t) denominationToSatoshi: (NSString *) amount;
- (NSString *)conversionStringFromNum:(int) currencyNum withAbbrev:(bool) abbrev;
- (BOOL)needsRecoveryQuestionsReminder;
- (BOOL)recentlyLoggedIn;
- (BOOL)passwordOk:(NSString *)password;
- (NSString *) bitidParseURI:(NSString *)uri;
- (BOOL) bitidLogin:(NSString *)uri;
- (BitidSignature *) bitidSign:(NSString *)uri msg:(NSString *)msg;


/// -----------------------------------------------------------------------------
/// @name Account Management
/// -----------------------------------------------------------------------------


/*
 * changePassword
 * @param NSString* password: new password for currently logged in user
 *
 * (Optional. If used, method returns immediately with ABCCConditionCodeOk)
 * @param completionHandler: completion handler code block
 * @param errorHandler: error handler code block which is called with the following args
 *                          @param ABCConditionCode       ccode: ABC error code
 *                          @param NSString *       errorString: error message
 * @return ABCConditionCode
 */
- (ABCConditionCode)changePassword:(NSString *)password;
- (ABCConditionCode)changePassword:(NSString *)password
                          complete:(void (^)(void)) completionHandler
                             error:(void (^)(ABCConditionCode ccode, NSString *errorString)) errorHandler;

/*
 * changePIN
 * @param NSString* pin: new pin for currently logged in user
 *
 * (Optional. If used, method returns immediately with ABCCConditionCodeOk)
 * @param completionHandler: completion handler code block
 * @param errorHandler: error handler code block which is called with the following args
 *                          @param ABCConditionCode       ccode: ABC error code
 *                          @param NSString *       errorString: error message
 * @return ABCConditionCode
 */
- (ABCConditionCode)changePIN:(NSString *)pin;
- (ABCConditionCode)changePIN:(NSString *)pin
                     complete:(void (^)(void)) completionHandler
                        error:(void (^)(ABCConditionCode ccode, NSString *errorString)) errorHandler;

/**
 * Check if this user has a password on the account or if it is
 * a PIN-only account.
 * @param username NSString* user to check
 * @return BOOL true if user has a password
 */
- (BOOL)passwordExists;


/// -----------------------------------------------------------------------------
/// @name Wallet Management
/// -----------------------------------------------------------------------------

/*
 * createWallet
 * @param NSString* walletName: set to nil to use default wallet name
 * @param int       currencyNum: ISO currency number for wallet. set to 0 to use defaultCurrencyNum from
 *                               settings or the global default currency number if settings unavailable
 *
 * (Optional. If used, method returns immediately with ABCCConditionCodeOk)
 * @param completionHandler: completion handler code block
 * @param errorHandler: error handler code block which is called with the following args
 *                          @param ABCConditionCode       ccode: ABC error code
 *                          @param NSString *       errorString: error message
 * @return ABCConditionCode
 */
- (ABCWallet *) createWallet:(NSString *)walletName currencyNum:(int) currencyNum;
- (void) createWallet:(NSString *)walletName currencyNum:(int) currencyNum
             complete:(void (^)(void)) completionHandler
                error:(void (^)(ABCConditionCode ccode, NSString *errorString)) errorHandler;


/*
 * renameWallet
 * @param NSString* walletUUID: UUID of wallet to rename
 * @param NSString*    newName: new name of wallet
 * @return ABCConditionCode
 */
- (ABCConditionCode) renameWallet:(NSString *)walletUUID
                          newName:(NSString *)walletName;

- (ABCConditionCode) createFirstWalletIfNeeded;
- (ABCConditionCode) getNumWalletsInAccount:(int *)numWallets;

- (BOOL)hasOTPResetPending;

/**
 * Gets the locally saved OTP key for the current user.
 * @return key NSString* OTP key
 */
- (NSString *)getOTPLocalKey;

/**
 * setOTPKey
 * Associates an OTP key with the given username.
 * This will not write to disk until the user has successfully logged in
 * at least once.
 * @param NSString* username: user to set the OTP key for
 * @param NSString*      key: key to set
 * @return ABCConditionCode
 */
- (ABCConditionCode)setOTPKey:(NSString *)username
                          key:(NSString *)key;

/**
 * removeOTPKey
 * Removes the OTP key for current user.
 * This will remove the key from disk as well.
 * @return ABCConditionCode
 */
- (ABCConditionCode)removeOTPKey;

/**
 * getOTPDetails
 * Reads the OTP configuration from the server.
 * This will remove the key from disk as well.
 * @param     bool*  enabled: enabled flag if OTP is enabled for this user
 * @param     long*  timeout: number seconds required after a reset is requested
 * @return ABCConditionCode
 */
- (ABCConditionCode)getOTPDetails:(bool *)enabled
                          timeout:(long *)timeout;

/**
 * setOTPAuth
 * Sets up OTP authentication on the server for currently logged in user
 * This will generate a new token if the username doesn't already have one.
 * @param     long   timeout: number seconds required after a reset is requested
 *                            before OTP is disabled.
 * @return ABCConditionCode
 */
- (ABCConditionCode)setOTPAuth:(long)timeout;

/**
 * removeOTPAuth
 * Removes the OTP authentication requirement from the server for the
 * currently logged in user
 * @return ABCConditionCode
 */
- (ABCConditionCode)removeOTPAuth;

/**
 * getOTPResetDateForLastFailedAccountLogin
 *
 * Returns the OTP reset date for the last account that failed to log in,
 * if any. Returns an empty string otherwise.
 * @param NSDate   **date: pointer to NSDate for return value date
 * @return ABCConditionCode
 */
- (ABCConditionCode)getOTPResetDateForLastFailedAccountLogin:(NSDate **)date;

/**
 * requestOTPReset
 * Launches an OTP reset timer on the server,
 * which will disable the OTP authentication requirement when it expires.
 *
 * This only works after the caller has successfully authenticated
 * with the server, such as through a password login,
 * but has failed to fully log in due to a missing OTP key.
 * @param NSString   *username:
 *
 * (Optional. If used, method returns immediately with ABCCConditionCodeOk)
 * @param completionHandler: completion handler code block
 * @param errorHandler: error handler code block which is called with the following args
 *                          @param ABCConditionCode       ccode: ABC error code
 *                          @param NSString *       errorString: error message
 * @return ABCConditionCode
 */
- (ABCConditionCode)requestOTPReset:(NSString *)username;
- (ABCConditionCode)requestOTPReset:(NSString *)username
                           complete:(void (^)(void)) completionHandler
                              error:(void (^)(ABCConditionCode ccode, NSString *errorString)) errorHandler;

/**
 * removeOTPResetRequest
 * Removes the OTP reset request from the server for the
 * currently logged in user
 * @return ABCConditionCode
 */
- (ABCConditionCode)removeOTPResetRequest;

/**
 * encodeStringToQRImage
 * Encodes a string into a QR code returned as UIImage *
 *
 * @param     NSString*   string: string to encode
 * @param     UIImage**    image: returned image
 * @return ABCConditionCode
 */
- (ABCConditionCode)encodeStringToQRImage:(NSString *)string
                                    image:(UIImage **)image;


/*
 * uploadLogs
 * @param NSString* userText: text to send to support staff
 *
 * (Optional. If used, method returns immediately with ABCCConditionCodeOk)
 * @param complete: completion handler code block which is called with void
 * @param error: error handler code block which is called with the following args
 *                          @param ABCConditionCode       ccode: ABC error code
 *                          @param NSString *       errorString: error message
 * @return ABCConditionCode
 */
- (ABCConditionCode)uploadLogs:(NSString *)userText;
- (ABCConditionCode)uploadLogs:(NSString *)userText
                      complete:(void(^)(void))completionHandler
                         error:(void (^)(ABCConditionCode ccode, NSString *errorString)) errorHandler;

/*
 * walletRemove
 * @param NSString* uuid: UUID of wallet to delete
 *
 * (Optional. If used, method returns immediately with ABCCConditionCodeOk)
 * @param complete: completion handler code block which is called with void
 * @param error: error handler code block which is called with the following args
 *                          @param ABCConditionCode       ccode: ABC error code
 *                          @param NSString *       errorString: error message
 * @return ABCConditionCode
 */

- (ABCConditionCode)walletRemove:(NSString *)uuid;
- (ABCConditionCode)walletRemove:(NSString *)uuid
                        complete:(void(^)(void))completionHandler
                           error:(void (^)(ABCConditionCode ccode, NSString *errorString)) errorHandler;

/*
 * setRecoveryQuestions
 * @param NSString* password: password of currently logged in user
 * @param NSString* questions: concatenated string of recovery questions separated by '\n' after each question
 * @param NSString* answers: concatenated string of recovery answers separated by '\n' after each answer
 *
 * (Optional. If used, method returns immediately with ABCCConditionCodeOk)
 * @param complete: completion handler code block which is called with void
 * @param error: error handler code block which is called with the following args
 *                          @param ABCConditionCode       ccode: ABC error code
 *                          @param NSString *       errorString: error message
 * @return ABCConditionCode
 */

- (ABCConditionCode)setRecoveryQuestions:(NSString *)password
                               questions:(NSString *)questions
                                 answers:(NSString *)answers;
- (ABCConditionCode)setRecoveryQuestions:(NSString *)password
                               questions:(NSString *)questions
                                 answers:(NSString *)answers
                                complete:(void (^)(void)) completionHandler
                                   error:(void (^)(ABCConditionCode ccode, NSString *errorString)) errorHandler;



/*
 * clearBlockchainCache
 * clears the local cache of blockchain info and force a re-download. This will cause wallets
 * to report incorrect balances which the blockchain is resynced
 *
 * @param complete: completion handler code block which is called with ABCSpend *
 *                          @param ABCSpend *    abcSpend: ABCSpend object
 * @param error: error handler code block which is called with the following args
 *                          @param ABCConditionCode       ccode: ABC error code
 *                          @param NSString *       errorString: error message
 * @return ABCConditionCodeOk (always returns Ok)
 */
- (ABCConditionCode)clearBlockchainCache;
- (ABCConditionCode)clearBlockchainCache:(void (^)(void)) completionHandler
                                   error:(void (^)(ABCConditionCode ccode, NSString *errorString)) errorHandler;


/*
 * satoshiToCurrency
 *      Convert bitcoin amount in satoshis to a fiat currency amount
 * @param uint_64t     satoshi: amount to convert in satoshis
 * @param int      currencyNum: ISO currency number of fiat currency to convert to
 * @param double    *pCurrency: pointer to resulting value
 * @return ABCConditionCode
 */
- (ABCConditionCode) satoshiToCurrency:(uint64_t) satoshi
                           currencyNum:(int)currencyNum
                              currency:(double *)pCurrency;

/*
 * currencyToSatoshi
 *      Convert fiat amount to a satoshi amount
 * @param double      currency: amount to convert in satoshis
 * @param int      currencyNum: ISO currency number of fiat currency to convert from
 * @param uint_64t   *pSatoshi: pointer to resulting value
 * @return ABCConditionCode
 */
- (ABCConditionCode) currencyToSatoshi:(double)currency
                           currencyNum:(int)currencyNum
                               satoshi:(int64_t *)pSatoshi;
- (ABCConditionCode) getLastConditionCode;
- (NSString *) getLastErrorString;

/*
 * shouldAskUserToEnableTouchID
 *  Evaluates if user should be asked to enable touch ID based
 *  on various factors such as if they have ever disabled touchID
 *  in the past, if they have touchID hardware support, and if
 *  this account has a password. PIN only accounts can't user TouchID
 *  at the moment. If user previously had touchID enabled, this will
 *  automatically enable touchID and return NO.
 *  Should be called while logged in.
 * @return BOOL: Should GUI ask if user wants to enable
 */
- (BOOL) shouldAskUserToEnableTouchID;

- (ABCConditionCode)accountDataGet:(NSString *)folder withKey:(NSString *)key data:(NSMutableString *)data;
- (ABCConditionCode)accountDataSet:(NSString *)folder withKey:(NSString *)key withValue:(NSString *)value;
- (ABCConditionCode)accountDataRemove:(NSString *)folder withKey:(NSString *)key;
- (ABCConditionCode)accountDataClear:(NSString *)folder;


//////////////////////////////////////////////////////////////////////////
// Internal routines - Do not call from GUI App
//////////////////////////////////////////////////////////////////////////
- (void)login;
- (void)logout;
- (void)enterBackground;
- (void)enterForeground;
- (BOOL)didLoginExpire;
- (void)postToGenQRQueue:(void(^)(void))cb;
- (void)postToMiscQueue:(void(^)(void))cb;
- (void)postToWatcherQueue:(void(^)(void))cb;
- (ABCConditionCode)setDefaultCurrencyNum:(int)currencyNum;
- (void)restoreConnectivity;
- (void)lostConnectivity;
- (void)setupLoginPIN;
- (void)watchAddresses: (NSString *) walletUUID;



@end
