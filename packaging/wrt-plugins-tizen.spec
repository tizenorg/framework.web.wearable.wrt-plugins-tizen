Name:       wrt-plugins-tizen
Summary:    JavaScript plugins for WebRuntime
Version:    0.5.38
Release:    0
Group:      Development/Libraries
License:    Apache License, Version 2.0, Samsung Properietary
Source0:    %{name}-%{version}.tar.gz

%define    tizen_profile_mobile    0
%define    tizen_profile_wearable    1
%define    tizen_profile_tv    0

%define    tizen_feature_core_api_support    0
%define    tizen_feature_impl_backward_compatible_support    1

# Tizen Web Device APIs
# Account API is optional in Tizen Wearable Profile.
%define    tizen_feature_account_support    0

# Archive API is optional in Tizen Wearable Profile.
%define    tizen_feature_archive_support    0

# Badge API is mandatory in Tizen Wearable Profile.
%define    tizen_feature_badge_support    1

%define    tizen_feature_bluetooth_support    0
%define    tizen_feature_bluetooth_health_support    0
%define    tizen_feature_bluetooth_always_on    0

# Bookmark API is optional in Tizen Wearable Profile.
%define    tizen_feature_bookmark_support    0

# Calendar API is mandatory in Tizen Wearable Profile.
%define    tizen_feature_calendar_support    0

# Contact API is optional in Tizen Wearable Profile.
%define    tizen_feature_contact_support    0

# Content API is mandatory in Tizen Wearable Profile.
%define    tizen_feature_content_support    1

# DataControl API is optional in Tizen Wearable Profile.
%define    tizen_feature_datacontrol_support    0

# DataSynchronization API is optional in Tizen Wearable Profile.
%define    tizen_feature_datasync_support    0

# Download API is optional in Tizen Wearable Profile.
%ifarch %{arm}
%define    tizen_feature_download_support    0
%else
%define    tizen_feature_download_support    1
%endif

# Exif API is optional in Tizen Wearable Profile.
%define    tizen_feature_exif_support    0

# FM Radio API is optional in Tizen Wearable Profile.
%define    tizen_feature_fm_radio_support    0

# HumanActivityMonitor(HAM) API is optional in Tizen Wearable Profile.
%define    tizen_feature_ham_support    1

# HumanActivityMonitor(HAM) - GPS type
%define    tizen_feature_location_batch    0

# MediayKey API is optional in Tizen Wearable Profile.
# tizen.org/feature/network.bluetooth.audio.media is required for MediayKey API
%ifarch %{arm}
%define    tizen_feature_mediakey_support    1
%else
%define    tizen_feature_mediakey_support    0
%endif

# MessagePort API is mandatory in Tizen Wearable Profile.
%define    tizen_feature_messageport_support    1

# Messaging API is optional in Tizen Wearable Profile.
%define    tizen_feature_messaging_support    0

# Messaging(Email) API is optional in Tizen Wearable Profile.
%define    tizen_feature_messaging_email_support    0

# NFC API is optional in Tizen Wearable Profile.
%define    tizen_feature_nfc_emulation_support    0
%define    tizen_feature_nfc_support    0

# Notification API is optional in Tizen Wearable Profile.
%define    tizen_feature_notification_support    0

# Power API is mandatory in Tizen Wearable Profile.
%define    tizen_feature_power_support    1

# Push API is optional in Tizen Wearable Profile.
%define    tizen_feature_push_support    0

# Sensor API is optional in Tizen Wearable Profile.
%define    tizen_feature_sensor_support    1

# Secure Element API is optional in Tizen Wearable Profile.
%define    tizen_feature_se_support    0

# Sound API is mandatory in Tizen Wearable Profile.
%define    tizen_feature_sound_support    1

# SystemSetting API is optional in Tizen Wearable Profile.
%define    tizen_feature_system_setting_support    1
%define    tizen_feature_system_setting_home_screen_support    1
%define    tizen_feature_system_setting_lock_screen_support    0
%define    tizen_feature_system_setting_incoming_call_screen_support    1
%define    tizen_feature_system_setting_notification_email_screen_support    0

# Telephony feature is optional in Tizen Wearable Profile.
%define    tizen_feature_telephony_support    0

# WebSetting API is optional in Tizen Wearable Profile.
%define    tizen_feature_web_setting_support    0

# Wi-Fi feature is optional in Tizen Wearable Profile.
%define    tizen_feature_wi_fi_support    1

#- telephony related APIs
# CallHistory API is optional in Tizen Wearable Profile.
# NetworkBearerSelection API is optional in Tizen Wearable Profile.
%if 0%{?tizen_feature_telephony_support}
%define    tizen_feature_callhistory_support    1
%define    tizen_feature_nbs_support    1
%else
%define    tizen_feature_callhistory_support    0
%define    tizen_feature_nbs_support    0
%endif

BuildRequires:  pkgconfig(ewebkit2)
BuildRequires:  pkgconfig(dpl-efl)
BuildRequires:  pkgconfig(dpl-event-efl)
BuildRequires:  pkgconfig(wrt-plugins-types)
BuildRequires:  pkgconfig(wrt-plugins-commons)
BuildRequires:  pkgconfig(wrt-plugins-commons-javascript)
BuildRequires:  pkgconfig(wrt-plugins-plugin-manager)
BuildRequires:  pkgconfig(wrt-plugins-ipc-message)
BuildRequires:  pkgconfig(glib-2.0)
BuildRequires:  pkgconfig(ecore)
BuildRequires:  pkgconfig(dbus-1)
BuildRequires:  pkgconfig(libpcrecpp)
BuildRequires:  pkgconfig(dlog)
BuildRequires:  pkgconfig(capi-appfw-application)
BuildRequires:  pkgconfig(capi-appfw-app-manager)
BuildRequires:  pkgconfig(capi-appfw-alarm)
BuildRequires:  pkgconfig(capi-appfw-package-manager)
BuildRequires:  pkgconfig(storage)
BuildRequires:  pkgconfig(pkgmgr)
BuildRequires:  pkgconfig(pkgmgr-info)
BuildRequires:  pkgconfig(capi-system-info)
BuildRequires:  pkgconfig(libxml-2.0)
BuildRequires:  pkgconfig(capi-content-media-content)
BuildRequires:  pkgconfig(capi-media-metadata-extractor)

BuildRequires:  pkgconfig(capi-system-device)

BuildRequires:  pkgconfig(capi-network-connection)
BuildRequires:  pkgconfig(capi-system-runtime-info)
BuildRequires:  pkgconfig(capi-system-sensor)
BuildRequires:  pkgconfig(sensor)
BuildRequires:  pkgconfig(capi-system-system-settings)
BuildRequires:  pkgconfig(icu-i18n)
BuildRequires:  pkgconfig(icu-io)
BuildRequires:  pkgconfig(icu-le)
BuildRequires:  pkgconfig(icu-lx)
BuildRequires:  pkgconfig(icu-uc)

BuildRequires:  pkgconfig(tapi)
BuildRequires:  pkgconfig(capi-system-device)
BuildRequires:  expat-devel
BuildRequires:  cmake
BuildRequires:  gettext-devel

%if 0%{?tizen_feature_account_support}
BuildRequires:  pkgconfig(accounts-svc)
%endif

%if 0%{?tizen_feature_archive_support}
BuildRequires:  pkgconfig(minizip)
%endif

%if 0%{?tizen_feature_badge_support}
BuildRequires:  pkgconfig(badge)
%endif

%if 0%{?tizen_feature_bluetooth_support}
BuildRequires:  pkgconfig(capi-network-bluetooth)
%endif

%if 0%{?tizen_feature_bookmark_support}
BuildRequires:  pkgconfig(capi-web-bookmark)
%endif

%if 0%{?tizen_feature_calendar_support}
BuildRequires:  pkgconfig(calendar-service2)
%endif

%if 0%{?tizen_feature_contact_support}
BuildRequires:  pkgconfig(contacts-service2)
%endif

%if 0%{?tizen_feature_datacontrol_support}
BuildRequires:  pkgconfig(capi-data-control)
%endif

%if 0%{?tizen_feature_datasync_support}
BuildRequires:  pkgconfig(sync-agent)
%endif

%if 0%{?tizen_feature_download_support}
BuildRequires:  pkgconfig(capi-web-url-download)
%endif

%if 0%{?tizen_feature_exif_support}
BuildRequires:  pkgconfig(libexif)
%endif

%if 0%{?tizen_feature_fm_radio_support}
BuildRequires:  pkgconfig(capi-media-radio)
%endif

%if 0%{?tizen_feature_ham_support}
BuildRequires:  pkgconfig(sensor)
BuildRequires:  pkgconfig(glib-2.0)
BuildRequires:  pkgconfig(libcore-context-manager)
%endif

%if 0%{?tizen_feature_mediakey_support}
BuildRequires:  pkgconfig(capi-system-media-key)
%endif

%if 0%{?tizen_feature_messageport_support}
BuildRequires:  pkgconfig(message-port)
%endif

%if 0%{?tizen_feature_messaging_email_support}
BuildRequires:  pkgconfig(email-service)
%endif

%if 0%{?tizen_feature_nfc_support}
BuildRequires:  pkgconfig(capi-network-nfc)
%endif

%if 0%{?tizen_feature_power_support}
BuildRequires:  pkgconfig(deviced)
%endif

%if 0%{?tizen_feature_push_support}
BuildRequires:  pkgconfig(push)
%endif

%if 0%{?tizen_feature_se_support}
BuildRequires:  pkgconfig(smartcard-service-common)
BuildRequires:  pkgconfig(smartcard-service)
%endif

%if 0%{?tizen_feature_sensor_support}
BuildRequires:  pkgconfig(sensor)
BuildRequires:  pkgconfig(capi-system-sensor)
%endif

%if 0%{?tizen_feature_sound_support}
BuildRequires:  pkgconfig(capi-media-sound-manager)
BuildRequires:  pkgconfig(vconf)
%endif

%if 0%{?tizen_feature_wi_fi_support}
BuildRequires:  pkgconfig(capi-network-wifi)
%endif

Requires:  wrt-plugins-common

%description
JavaScript plugins for WebRuntime

%package devel
Summary:    Wrt-plugin-tizen development headers
Group:      Development/Libraries
Requires:   %{name} = %{version}

%description devel
Wrt-plugin-tizen development headers

%prep
%setup -q

%build

%define PREFIX "%{_libdir}/wrt-plugins"

export LDFLAGS+="-Wl,--rpath=%{PREFIX} -Wl,--as-needed"

# enable debugging
export CFLAGS="$CFLAGS -DTIZEN_DEBUG_ENABLE"
export CXXFLAGS="$CXXFLAGS -DTIZEN_DEBUG_ENABLE"
export FFLAGS="$FFLAGS -DTIZEN_DEBUG_ENABLE"

cmake \
%if 0%{?tizen_profile_mobile}
-DENABLE_TIZEN_MOBILE_PROFILE=YES \
%endif
%if 0%{?tizen_profile_wearable}
-DENABLE_TIZEN_WEARABLE_PROFILE=YES \
%endif
%if 0%{?tizen_profile_tv}
-DENABLE_TIZEN_TV_PROFILE=YES \
%endif
%if 0%{?tizen_feature_impl_backward_compatible_support}
-DENABLE_IMPL_BACKWARD_COMPATIBLE=YES \
%endif
%if 0%{?tizen_feature_core_api_support}
-DENABLE_OPTIONAL_CORE_API=YES \
%endif
%if 0%{?tizen_feature_account_support}
-DENABLE_OPTIONAL_ACCOUNT=YES \
%endif
%if 0%{?tizen_feature_archive_support}
-DENABLE_OPTIONAL_ARCHIVE=YES \
%endif
%if 0%{?tizen_feature_badge_support}
-DENABLE_OPTIONAL_BADGE=YES \
%endif
%if 0%{?tizen_feature_bluetooth_support}
-DENABLE_OPTIONAL_BT=YES \
%endif
%if 0%{?tizen_feature_bluetooth_health_support}
-DENABLE_OPTIONAL_BT_HEALTH=YES \
%endif
%if 0%{?tizen_feature_bluetooth_always_on}
-DENABLE_OPTIONAL_BT_ALWAYS_ON=YES \
%endif
%if 0%{?tizen_feature_bookmark_support}
-DENABLE_OPTIONAL_BOOKMARK=YES \
%endif
%if 0%{?tizen_feature_calendar_support}
-DENABLE_OPTIONAL_CALENDAR=YES \
%endif
%if 0%{?tizen_feature_callhistory_support}
-DENABLE_OPTIONAL_CALL_HISTORY=YES \
%endif
%if 0%{?tizen_feature_contact_support}
-DENABLE_OPTIONAL_CONTACT=YES \
%endif
%if 0%{?tizen_feature_content_support}
-DENABLE_OPTIONAL_CONTENT=YES \
%endif
%if 0%{?tizen_feature_datacontrol_support}
-DENABLE_OPTIONAL_DATA_CONTROL=YES \
%endif
%if 0%{?tizen_feature_datasync_support}
-DENABLE_OPTIONAL_DATA_SYNC=YES \
%endif
%if 0%{?tizen_feature_download_support}
-DENABLE_OPTIONAL_DOWNLOAD=YES \
%endif
%if 0%{?tizen_feature_exif_support}
-DENABLE_OPTIONAL_EXIF=YES \
%endif
%if 0%{?tizen_feature_fm_radio_support}
-DENABLE_OPTIONAL_FM_RADIO=YES \
%endif
%if 0%{?tizen_feature_ham_support}
-DENABLE_OPTIONAL_HAM=YES \
%endif
%if 0%{?tizen_feature_location_batch}
-DENABLE_OPTIONAL_LOCATION_BATCH=YES \
%endif
%if 0%{?tizen_feature_mediakey_support}
-DENABLE_OPTIONAL_MEDIAKEY=YES \
%endif
%if 0%{?tizen_feature_messageport_support}
-DENABLE_OPTIONAL_MESSAGE_PORT=YES \
%endif
%if 0%{?tizen_feature_messaging_support}
-DENABLE_OPTIONAL_MESSAGING=YES \
%endif
%if 0%{?tizen_feature_messaging_email_support}
-DENABLE_OPTIONAL_MESSAGING_EMAIL=YES \
%endif
%if 0%{?tizen_feature_nbs_support}
-DENABLE_OPTIONAL_NBS=YES \
%endif
%if 0%{?tizen_feature_nfc_support}
-DENABLE_OPTIONAL_NFC=YES \
%endif
%if 0%{?tizen_feature_nfc_emulation_support}
-DENABLE_OPTIONAL_NFC_EMULATION=YES \
%endif
%if 0%{?tizen_feature_notification_support}
-DENABLE_OPTIONAL_NOTIFICATION=YES \
%endif
%if 0%{?tizen_feature_power_support}
-DENABLE_OPTIONAL_POWER=YES \
%endif
%if 0%{?tizen_feature_push_support}
-DENABLE_OPTIONAL_PUSH=YES \
%endif
%if 0%{?tizen_feature_se_support}
-DENABLE_OPTIONAL_SE=YES \
%endif
%if 0%{?tizen_feature_sensor_support}
-DENABLE_OPTIONAL_SENSOR=YES \
%endif
%if 0%{?tizen_feature_sound_support}
-DENABLE_OPTIONAL_SOUND=YES \
%endif
%if 0%{?tizen_feature_system_setting_support}
-DENABLE_OPTIONAL_SYSTEM_SETTING=YES \
%endif
%if 0%{?tizen_feature_system_setting_home_screen_support}
-DENABLE_OPTIONAL_SYSTEM_SETTING_HOME_SCREEN=YES \
%endif
%if 0%{?define tizen_feature_system_setting_lock_screen_support}
-DENABLE_OPTIONAL_SYSTEM_SETTING_LOCK_SCREEN=YES \
%endif
%if 0%{?ttizen_feature_system_setting_incoming_call_screen_support}
-DENABLE_OPTIONAL_SYSTEM_SETTING_INCOMING_CALL=YES \
%endif
%if 0%{?ttizen_feature_system_setting_notification_email_screen_support}
-DENABLE_OPTIONAL_SYSTEM_SETTING_EMAIL_SCREEN=YES \
%endif
%if 0%{?tizen_feature_telephony_support}
-DENABLE_OPTIONAL_TELEPHONY=YES \
%endif
%if 0%{?tizen_feature_web_setting_support}
-DENABLE_OPTIONAL_WEB_SETTING=YES \
%endif
%if 0%{?tizen_feature_wi_fi_support}
-DENABLE_OPTIONAL_WI_FI=YES \
%endif
-DCMAKE_INSTALL_PREFIX=%{_prefix} -DDPL_LOG="OFF" -DENABLE_TIME_TRACER="OFF"

make %{?jobs:-j%jobs}

%install
rm -rf %{buildroot}
%make_install
mkdir -p %{buildroot}/usr/share/license
cp LICENSE %{buildroot}/usr/share/license/%{name}

%post

%postun


%files
%manifest wrt-plugins-tizen.manifest 
%{_libdir}/wrt-plugins/*
/usr/etc/tizen-apis/*
/usr/share/license/*

%files devel
%{_includedir}/*
%{_libdir}/pkgconfig/*
