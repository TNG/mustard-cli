## Changelog for mustard-cli
# 1.0.0
- feature: *mustard inbox* now can select PRs in same repo; will guide through fetching, checkout and starting review on PR.
- fix: standard branch in git is main now
- fix: travis ci
# 0.4.1
- update dependencies
- improve config guesser
# 0.4.0
- feature: mustard passwd - ask user whether to change username, too
- fix: feature branch could not be determined when commit msg contains an asterisk
# 0.3.0
- feature: *mustard inbox* can display all your currently pending PRs
- fix: libsecret would not ask for password in case of a ["cold keyring"](https://gitlab.gnome.org/GNOME/libsecret/issues/7)
- fix: insufficient debug output