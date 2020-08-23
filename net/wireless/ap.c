#include <linux/ieee80211.h>
#include <linux/export.h>
#include <net/cfg80211.h>
#include "nl80211.h"
#include "core.h"
#include "rdev-ops.h"


<<<<<<< HEAD
<<<<<<< HEAD
static int __cfg80211_stop_ap(struct cfg80211_registered_device *rdev,
			      struct net_device *dev)
=======
int __cfg80211_stop_ap(struct cfg80211_registered_device *rdev,
		       struct net_device *dev, bool notify)
>>>>>>> v3.18
=======
int __cfg80211_stop_ap(struct cfg80211_registered_device *rdev,
		       struct net_device *dev, bool notify)
>>>>>>> v3.18
{
	struct wireless_dev *wdev = dev->ieee80211_ptr;
	int err;

	ASSERT_WDEV_LOCK(wdev);

	if (!rdev->ops->stop_ap)
		return -EOPNOTSUPP;

	if (dev->ieee80211_ptr->iftype != NL80211_IFTYPE_AP &&
	    dev->ieee80211_ptr->iftype != NL80211_IFTYPE_P2P_GO)
		return -EOPNOTSUPP;

	if (!wdev->beacon_interval)
		return -ENOENT;

	err = rdev_stop_ap(rdev, dev);
<<<<<<< HEAD
<<<<<<< HEAD
	wdev->beacon_interval = 0;
	if (!err) {
		wdev->channel = NULL;
		wdev->ssid_len = 0;
		rdev_set_qos_map(rdev, dev, NULL);
=======
=======
>>>>>>> v3.18
	if (!err) {
		wdev->beacon_interval = 0;
		memset(&wdev->chandef, 0, sizeof(wdev->chandef));
		wdev->ssid_len = 0;
		rdev_set_qos_map(rdev, dev, NULL);
		if (notify)
			nl80211_send_ap_stopped(wdev);
<<<<<<< HEAD
>>>>>>> v3.18
=======
>>>>>>> v3.18
	}

	return err;
}

int cfg80211_stop_ap(struct cfg80211_registered_device *rdev,
<<<<<<< HEAD
<<<<<<< HEAD
		     struct net_device *dev)
=======
		     struct net_device *dev, bool notify)
>>>>>>> v3.18
=======
		     struct net_device *dev, bool notify)
>>>>>>> v3.18
{
	struct wireless_dev *wdev = dev->ieee80211_ptr;
	int err;

	wdev_lock(wdev);
<<<<<<< HEAD
<<<<<<< HEAD
	err = __cfg80211_stop_ap(rdev, dev);
=======
	err = __cfg80211_stop_ap(rdev, dev, notify);
>>>>>>> v3.18
=======
	err = __cfg80211_stop_ap(rdev, dev, notify);
>>>>>>> v3.18
	wdev_unlock(wdev);

	return err;
}
