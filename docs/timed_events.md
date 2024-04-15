# Timed Events

The `bmx` Timed Events is a preliminary implementation for embedding text-based timed events in MXF that is based on a draft specification of SMPTE ST 2067-207. This specification forms part of the Interoperable Master Format (IMF) set of SMPTE specifications.

The Timed Events are embedded in the same way as [Timed Text](./timed_text.md) and consequently much of the implementation code is shared. The implementation code uses the Timed Data naming when combining the implementation code for Timed Text and Timed Events.

The description for [Timed Text](./timed_text.md) mostly applies to Timed Events as well, except for these differences:
* The manifest file format uses the XML representation. It is described below in the [Manifest File Format](#manifest-file-format) section.
* The `raw2bmx` option is `--timed-events`.
* The time offset applied in the Material Package to position the Timed Text is named `timed_events_offset` in the `mxf2raw` output.
* The Timed Events metadata is slightly different. The [Commandline Utilities: `mxf2raw`](#commandline-utilities-mxf2raw) sections provides an example output from `mxf2raw`.
* The manifest class is `TimedEventsManifest`.
* The method to read ancillary resources by ID is `MXFTimedDataTrackReader::ReadTimedEventsAncillaryResourceById()`.


### Manifest File Format

The manifest file provides `raw2bmx` metadata and locations for timed events and ancillary resources. It is represented in XML and defined in the [timed_events_manifest.xsd](../meta/timed_events_manifest.xsd) XML Schema.

The XML namespace is "http://bbc.co.uk/bmx/timed-events/202403" and root element is `<manifest>`.

The `<file>` element (*required*) provides information about the Timed Events essence and the position in the Material Package:
* `path` (*required*): The file location of the Timed Events essence. A relative file location is relative to the location of the manifest.
* `mime_type` (*required*): The MIME type for the Timed Events essence. This attribute maps to the `TextMIMEType` property in the `TimedEventsTextDescriptor` class.
* `mime_encoding` (*required*): The MIME text encoding for the Timed Events essence. This attribute maps to the `TextMIMEEncoding` property in the `TimedEventsTextDescriptor` class.
* `start` (*optional*): Specifies the non-zero start position for the Timed Events. The value is either a timecode (HH:MM:SS:FF or HH:MM:SS;FF) or a position in media edit rate. It is assumed to be 0 if not set.

The `<file>` element contains a `<event_schemes>` (*required*) that contains 1 or more URIs in `<uri>` elements. These URIs map to elements of the `EventSchemeURIs` property in the `TimedEventsTextDescriptor` class.

The `<ancillary_resources>` element (*optional*) lists ancillary resource data files in `resource` elements:
* `path` (*required*): The file location of the ancillary resource data. A relative file location is relative to the location of the manifest.
* `id` (*optional*): The resource identifier. The default is the filename (name plus suffix) component of the file path. This attribute maps to the `TimedEventsAncillaryResourceID` property of the `TimedEventsAncillaryResourceSubDescriptor` class.
* `mime_type` (*required*): The MIME type for the ancillary data. This attribute maps to the `TimedEventsAncillaryResourceMIMEType` property in the `TimedEventsAncillaryResourceSubDescriptor` class.

The `video_viewports` element (*optional*) provides metadata for the Video Viewports Simple Scheme. It lists the available experiences identifiers in the `<id>` child element in the `<available_experiences>` element. These identifiers map to elements of the `AvailableExperiencesList` property in the `VideoViewportsSubDescriptor` class.

An example command and manifest is shown below for creating an IMF file containing Video Viewports Timed Events represented in XML. The Timed Events use the "broadcaster-x" scheme that is based on the Simple Scheme (SMPTE ST 2067-206). The Timed Events are positioned at the start of the programme at 10:00:00:00.

The `raw2bmx` command to create the IMF file given a manifest file, `manifest.xml`, and ancillary resource image file, `image-104.png`:

`raw2bmx -t imf -f 25 --dur 100 --timed-events manifest.xml`

`manifest.xml`:
```xml
<?xml version="1.0" encoding="UTF-8"?>
<manifest xmlns="http://bbc.co.uk/bmx/timed-events/202403">
    <file path="viewports_example.xml" mime_type="application/xml" mime_encoding="UTF-8" start="10:00:00:00">
        <event_schemes>
            <uri>http://www.smpte-ra.org/ns/2067-206/2023/event-scheme</uri>
            <uri>urn:broadcaster-x:events:viewports</uri>
        </event_schemes>
    </file>
    <ancillary_resources>
        <resource path="image-104.png" id="goal1.png" mime_type="image/png"/>
    </ancillary_resources>
    <video_viewports>
        <available_experiences>
            <id>X01</id>
        </available_experiences>
    </video_viewports>
</manifest>
```

### Commandline Utilities: `mxf2raw`

The `mxf2raw` utility can be used to show metadata about the Timed Events tracks and for extracting the essence data to files. The Timed Events tracks will have metadata shown similar to the extract below. It shows the properties in the Timed Events data file descriptor and sub-descriptors. A non-zero Timed Events offset, which corresponds to the `start` field in the manifest, is shown in the `timed_events_offset` field in the Track information.

```text
  Tracks: (1)
    Track #0:
      essence_kind    : Data
      essence_type    : Timed_Events
      ec_label        : urn:smpte:ul:060e2b34.0401010d.0d010301.02130102
      edit_rate       : 25/1
      duration        : 00:27:14:08 (count='40858')
      timed_events_offset : 00:00:01:00 (count='25')
      Packages: (1)
        Package #0:
          Material:
            package_uid     : urn:smpte:umid:060a2b34.01010105.01010f20.13000000.b5b1ecf3.a9c44492.89bf6627.d60f8496
            track_id        : 3001
            track_number    : 0
          FileSource:
            package_uid     : urn:smpte:umid:060a2b34.01010105.01010f20.13000000.6ccf5a20.2d384b4e.9cc6c1f6.c3258a2a
            track_id        : 3001
            track_number    : 0x17011001
            file_uri        : file:///tmp/example.mxf
      DataDescriptor:
        TimedEventsDescriptor:
          mime_type       : application/xml
          mime_encoding   : UTF-8
          EventSchemes: (2)
            Element #0:
              uri             : http://www.smpte-ra.org/ns/2067-206/2023/event-scheme
            Element #1:
              uri             : urn:broadcaster:events:viewports
          ViewportExperiences: (1)
            Element #0:
              id              : X01
```
