/*
    Copyright (C) 2014-2016 Islog

    This file is part of Leosac.

    Leosac is free software: you can redistribute it and/or modify
    it under the terms of the GNU Affero General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Leosac is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#include "AuditEntry.hpp"
#include "ICredentialEvent.hpp"
#include "core/credentials/CredentialFwd.hpp"

namespace Leosac
{
namespace Audit
{
/**
 * Provides an implementation of ICredentialEvent.
 */
#pragma db object polymorphic callback(odb_callback)
class CredentialEvent : virtual public ICredentialEvent, public AuditEntry
{
  private:
    CredentialEvent() = default;

    friend class Factory;

    static std::shared_ptr<CredentialEvent> create(const DBPtr &database,
                                                   Cred::ICredentialPtr target_cred,
                                                   AuditEntryPtr parent);

  public:
    virtual ~CredentialEvent() = default;

    virtual void target(Cred::ICredentialPtr cred) override;

    virtual void before(const std::string &repr) override;

    virtual void after(const std::string &repr) override;

  public:
#pragma db on_delete(set_null)
    Cred::CredentialLWPtr target_;

    Cred::CredentialId target_cred_id_;

    /**
     * Optional JSON dump of the object before the event took place.
     */
    std::string before_;

    /**
     * Optional JSON dump of the object after the event took place.
     */
    std::string after_;

    friend class odb::access;
};
}
}

#ifdef ODB_COMPILER
#include "core/credentials/Credential.hpp"
#endif
