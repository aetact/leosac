/*
    Copyright (C) 2014-2016 Leosac

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

// Prevent the AuditTracker header from including
// required header in the end of the file. Instead we'll include
// them at the end of this file.
#define LEOSAC_AUDIT_TRACKER_NO_ODB_CYCLE
#include "core/audit/AuditTracker.hpp"
#undef LEOSAC_AUDIT_TRACKER_NO_ODB_CYCLE

#include "core/update/IUpdate.hpp"
#include <odb/core.hxx>
#include <string>

namespace Leosac
{
namespace update
{

/**
* Represent an update.
*
* This is a base class for updates. It is intended to be
* extended by concrete update type, such as hardware update.
*/
#pragma db object polymorphic optimistic
class Update : virtual public IUpdate
{
  public:
    Update();

    virtual UpdateId id() const override;

    virtual const TimePoint &generated_at() const override;

    virtual Status status() const override;
    virtual void status(Status st) override;

    virtual const TimePoint &status_updated_at() const override;

    virtual void set_checkpoint(Audit::AuditEntryPtr) override;

    virtual Audit::AuditEntryId get_checkpoint() const override;

    virtual const std::string &source_module() const override;

    virtual const std::string &description() const override;

  private:
#pragma db id auto
    UpdateId id_;

#pragma db type("TIMESTAMP")
    TimePoint generated_at_;

    /**
     * Tracks the audit entry that represents the point in time
     * of this update.
     */
    Audit::AuditTracker checkpoint_;

/**
 * Last timepoint when status was updated.
 *
 * Normally we should only change status once.
 */
#pragma db type("TIMESTAMP")
    TimePoint status_updated_at_;

    /**
     * The name (as returned by get_module_name()) that
     * created this update.
     *
     * Should be "LEOSAC" for update generated by the core system.
     */
    std::string source_module_;

    /**
     * Optional description for the update.
     */
    std::string description_;

    Status status_;

#pragma db version
    const size_t odb_version_;

    friend class odb::access;
};
}
}

// This is required by AuditTracker.hpp, but we have to include
// it here, otherwise we hit cyclic dependencies.
#if defined(ODB_COMPILER)
#include "core/audit/AuditEntry.hpp"
#endif
