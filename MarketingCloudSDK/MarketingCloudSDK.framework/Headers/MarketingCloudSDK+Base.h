//
//  MarketingCloudSDK+Base.h
//  MarketingCloudSDK
//
//  https://salesforce-marketingcloud.github.io/MarketingCloudSDK-iOS/
//  Copyright © 2017 Salesforce, Inc. All rights reserved.
//
#import <UIKit/UIKit.h>
#import <Foundation/Foundation.h>
#import <UserNotifications/UserNotifications.h>
#import <os/log.h>

NS_ASSUME_NONNULL_BEGIN

/** The MarketingCloudSDK base class and categories */
@interface MarketingCloudSDK : NSObject
- (instancetype)init NS_UNAVAILABLE;

/**
 Returns (or initializes) the shared MarketingCloudSDK instance.

 @return The singleton instance of the MarketingCloudSDK.
 */
+(instancetype)sharedInstance;

/**
 This is the main configuration method, responsible for setting credentials needed to communicate with Salesforce. You must have a JSON file embedded in the application bundle with optional parameter settings as part of your project. Settings that are absent will default to NO. See documentation for example JSON
 
 @param error NSError object describing the error
 @param completionHandler Called when the asynchronous portion has completed. Do not proceed if NO is returned and error will contain an error object describing the error.
 @return Returns YES if the synchronous portion is successful or NO if failed. Do not proceed if NO is returned and error will contain an error object describing the error.
 */
-(BOOL)sfmc_configure:(NSError * _Nullable *)error completionHandler:(void (^)(BOOL configured, NSString *appId, NSError * _Nullable error))completionHandler;

/**
 This is the main configuration method, responsible for setting credentials needed to communicate with Salesforce. You must pass a URL to a JSON file with optional parameter settings. Settings that are absent will default to NO. See documentation for example JSON.
 
 @param url URL to a JSON configuration file that is embedded in the application bundle.
 @param configurationIndex zero based index into into the JSON file array of configuration settings
 @param error NSError object describing the error
 @param completionHandler Called when the asynchronous portion has completed. Do not proceed if NO is returned and error will contain an error object describing the error.
 @return Returns YES if the synchronous portion is successful or NO if failed. Do not proceed if NO is returned and error will contain an error object describing the error.
 */
-(BOOL)sfmc_configureWithURL:(NSURL *)url configurationIndex:(NSNumber *) configurationIndex error:(NSError **)error completionHandler:(void (^)(BOOL configured, NSString *appId, NSError *completionError))completionHandler;

/** this method properly closes down the MarketingCloudSDK. It should be used in any cases where references to the MarketingCloudSDK need to be released.
 */
- (void) sfmc_tearDown;

/**
 Accepts and sets the Contact Key for the device's user. Formerly know in the SDK as "subscriberKey".
 
 Cannot be nil or blank.
 
 Will trim leading and trailing whitespace.
 
 @param contactKey The contact key to attribute to the user.
 @return YES if set successfully.
 */
-(BOOL)sfmc_setContactKey:( NSString * _Nonnull )contactKey;

/**
 Returns the contact key for the active user, in case you need it.
 
 @return contactKey The code-set contact key.
 */
-(NSString * _Nullable)sfmc_contactKey;

/**
 Adds the provided Tag (NSString) to the set of unique tags.
 
 Will trim leading and trailing whitespace.
 
 Cannot be nil or blank.
 
 @param  tag A string to add to the list of tags.
 @return YES if added successfully.
 */
-(BOOL)sfmc_addTag:(NSString * _Nonnull)tag;

/**
 Removes the provided Tag (NSString) from the list of tags.
 
 @param tag A string to remove from the list of tags.
 @return tag Echoes the tag back on successful removal, or nil if something failed.
 */
-(BOOL)sfmc_removeTag:(NSString * _Nonnull)tag;

/**
 Adds the provided array of Tags (NSString) to the set of unique tags.
 
 Will trim leading and trailing whitespace.
 
 Cannot be nil or blank.
 
 @param tags An array of tags to add to the list.
 @return Set of tags added, as strings, or nil if something failed.
 */
-(NSSet * _Nullable)sfmc_addTags:( NSArray * _Nonnull )tags;

/**
 Removes the provided array of Tags (NSString) from the list of tags.
 
 @param tags An array of tags to removed from the list.
 @return Set of tags removed upon success, as strings, or nil if something failed.
 */
-(NSSet * _Nullable)sfmc_removeTags:( NSArray * _Nonnull )tags;

/**
 Returns the list of tags for this device.
 
 @return All tags associated, as strings.
 */
-(NSSet * _Nullable)sfmc_tags;


/**
 Set an attribute to the data set sent to Salesforce.
 
 The Attribute Name cannot be nil or blank, or one of the reserved words.
 
 Will trim leading and trailing whitespace from the name and value.
 
 The attribute must be defined within the SFMC Contact model prior to setting a value. If the attribute does not exist within the
 SFMC Contact model, then this attribute will be accepted by the SDK, but will be ignored within the SFMC.
 
 If you previously set a value for the named attribute, then the value will be updated with the new value and sent to the SFMC.
 
 If you send in a blank value, then the value will be sent to the SFMC to remove that value from the Contact record.
 
 All attribute values set with this method persist through the installation of the app on your customer device.
 
 Note that attribute mapping is case sensitive, and spaces should be avoided when setting up new attributes in the SFMC Contact model.
 
 @param name The name of the attribute you wish to send. This will be the key of the pair.
 @param value The value to set for the data pair.
 @return YES if set successfully.
 */
- (BOOL)sfmc_setAttributeNamed:( NSString * _Nonnull )name value:( NSString * _Nonnull )value;

/**
 Removes the named attribute from the data set to send to Salesforce. The value is not changed on the SFMC.
 
 @param name The name of the attribute you wish to remove.
 @return Returns the value that was set. It will no longer be sent back to Salesforce.
 */
- (BOOL)sfmc_clearAttributeNamed:( NSString * _Nonnull )name;

/**
 Returns a read-only copy of the Attributes dictionary as it is right now.
 
 @return All attributes currently set.
 */
-(NSDictionary * _Nullable)sfmc_attributes;

/**
 Set multiple attributes (key/value dictionaries) to Salesforce. See comments in -sfmc_setAttributeNamed.
 
 @param attributes An array of dictionaries of key (attribute name) and value (attribute value).
 @return A set of all attributes set.
 */
- (NSDictionary * _Nullable) sfmc_setAttributes:( NSArray * _Nonnull ) attributes;

/**
 Remove multiple attributes from Salesforce. See comments in -sfmc_setAttributeNamed.
 
 @param attributeNames An array of attribute names.
 @return A set of all attributes removed.
 */
- (NSDictionary * _Nullable) sfmc_clearAttributesNamed:( NSArray * _Nonnull ) attributeNames;

/**
 Responsible for sending the Apple device token back to Salesforce. It marks the end of the token registration flow. If it is unable to reach Salesforce server, it will save the token and try again later.
 
 This method is necessary to the implementation of Salesforce Push.
 
 @param deviceToken Token as received from Apple
 
 */
-(void) sfmc_setDeviceToken:(NSData * _Nonnull)deviceToken;

/**
 Returns the device token as a NSString.
 
 @return NSData A stringified version of the Apple deviceToken
 
 */
- (NSString * _Nullable) sfmc_deviceToken;

/**
 Returns the Salesforce application ID.
 
 @return NSString Salesforce application ID
 */
- (NSString * _Nullable) sfmc_appID;

/**
 Returns the Salesforce application accessToken.
 
 @return NSString Salesforce application accessToken
 */
- (NSString * _Nullable)sfmc_accessToken;

/**
 Returns the unique device identifier that Salesforce will use to identify the device.

 @return NSString The device identifier (a UUID) as a NSString
 */
- (NSString * _Nullable) sfmc_deviceIdentifier;

/**
 Informs the SDK of the current notification

 @param request The UNNotificationRequest that generated a notification
 */
- (void)sfmc_setNotificationRequest:(UNNotificationRequest *)request;

/**
 Returns the last notification delivered to the SDK

 @return UNNotificationRequest * The last UNNotificationRequest that generated a notification
 */
- (UNNotificationRequest *)sfmc_notificationRequest;

/**
 Allows setting the state of pushEnabled to YES/NO. If set to NO, the application will not receive any push notifications

 @param pushEnabled Set to YES to enable push notifications.
 */
- (void)sfmc_setPushEnabled:(BOOL)pushEnabled;

/**
 The current state of the pushEnabled flag in the SDK

 @return returns a BOOL value of the current pushEnabled state
 */
- (BOOL)sfmc_pushEnabled;

/**
 Outputs a formatted, easily readable block of text describing the current status of the SDK.
 
 @return JSON string with values of the current state of the SDK
 
 */
-(nullable NSString *)sfmc_getSDKState NS_SWIFT_NAME(sfmc_getSDKState());

/**
 Enable/Disable extra debug logging from the SDK

 @param enabled BOOL for enabling or disabling extra SDK logging.
 */
- (void)sfmc_setDebugLoggingEnabled:(BOOL)enabled;

/**
 The current state of the debug logging flag in the SDK
 
 @return returns a BOOL value of the current debug logging state
 */
- (BOOL)sfmc_getDebugLoggingEnabled;
/**
 Ask MarketingCloudSDK to update its data. MarketingCloudSDK will throttle attempts based on the time since the last time this was called.
 
 @param completionHandler The UIBackgroundFetchResult completion handler. This method will be called with UIBackgroundFetchResultNoData if no attempt was made to update data, otherwise it will be called with UIBackgroundFetchResultNewData after the update completes. If nil is passed, then process of the completion handler must be managed by the caller.
 @return YES if MarketingCloudSDK did make an attempt at updating data
 */
- (BOOL) sfmc_refreshWithFetchCompletionHandler:(void (^_Nullable)(UIBackgroundFetchResult result))completionHandler;

@end

NS_ASSUME_NONNULL_END
