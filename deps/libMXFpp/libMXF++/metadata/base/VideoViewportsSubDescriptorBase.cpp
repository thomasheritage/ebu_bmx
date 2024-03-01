/*
 * Copyright (C) 2024, British Broadcasting Corporation
 * All Rights Reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright notice,
 *       this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the British Broadcasting Corporation nor the names
 *       of its contributors may be used to endorse or promote products derived
 *       from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <memory>

#include <libMXF++/MXF.h>
#include <libMXF++/Utils.h>

using namespace std;
using namespace mxfpp;


const mxfKey VideoViewportsSubDescriptorBase::setKey = MXF_SET_K(VideoViewportsSubDescriptor);


VideoViewportsSubDescriptorBase::VideoViewportsSubDescriptorBase(HeaderMetadata *headerMetadata)
: SubDescriptor(headerMetadata, headerMetadata->createCSet(&setKey))
{
    headerMetadata->add(this);
}

VideoViewportsSubDescriptorBase::VideoViewportsSubDescriptorBase(HeaderMetadata *headerMetadata, ::MXFMetadataSet *cMetadataSet)
: SubDescriptor(headerMetadata, cMetadataSet)
{}

VideoViewportsSubDescriptorBase::~VideoViewportsSubDescriptorBase()
{}

bool VideoViewportsSubDescriptorBase::haveAvailableExperiencesList() const
{
    return haveItem(&MXF_ITEM_K(VideoViewportsSubDescriptor, AvailableExperiencesList));
}

string VideoViewportsSubDescriptorBase::getAvailableExperiencesList() const
{
    return getStringItem(&MXF_ITEM_K(VideoViewportsSubDescriptor, AvailableExperiencesList));
}

vector<string> VideoViewportsSubDescriptorBase::getAvailableExperiencesListAsVector() const
{
    return split_string(getAvailableExperiencesList(), ',', false, true);
}

void VideoViewportsSubDescriptorBase::setAvailableExperiencesList(const string &value)
{
    setStringItem(&MXF_ITEM_K(VideoViewportsSubDescriptor, AvailableExperiencesList), value);
}

void VideoViewportsSubDescriptorBase::setAvailableExperiencesList(const vector<string> &value)
{
    // Concatenate values into a string using a comma as a separator
    string str_value;
    for (size_t i = 0; i < value.size(); i++) {
        if (i > 0)
            str_value += ",";
        str_value += value[i];
    }

    setAvailableExperiencesList(str_value);
}
