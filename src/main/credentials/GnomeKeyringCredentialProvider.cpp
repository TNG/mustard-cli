#include <Provide.h>
#include <glib.h>
#include "GnomeKeyringCredentialProvider.h"
#include "CredentialProvider.h"

#ifndef WITH_GNOME_KEYRING
ProvideDepedendency<CredentialProvider> credentialProvider;
#endif

#ifdef WITH_GNOME_KEYRING
#include <gnome-keyring.h>
#include <Depend.h>

ProvideImplementationForDependency<GnomeKeyringCredentialProvider, CredentialProvider> keyringCredentialProvider;

GnomeKeyringCredentialProvider::GnomeKeyringCredentialProvider ( BitBucketConfiguration *bitBucketConfiguration ) :
    bitBucketConfiguration ( DependentOn<BitBucketConfiguration> ( bitBucketConfiguration ) )
{
}

Credentials GnomeKeyringCredentialProvider::getCredentialsFor ( const string &serverName )
{

}

bool GnomeKeyringCredentialProvider::saveCredentials ( const Credentials &credentials )
{
    gnome_keyring_store_password_sync (
        GNOME_KEYRING_NETWORK_PASSWORD,
        nullptr,
        bitBucketConfiguration->getBitBucketEndpoint().c_str(),
        credentials.password.c_str(),
        "username",
        credentials.username.c_str()
    );
}

bool GnomeKeyringCredentialProvider::isReady()
{
    return true;
}

GnomeKeyringCredentialProvider::~GnomeKeyringCredentialProvider() = default;

#endif
