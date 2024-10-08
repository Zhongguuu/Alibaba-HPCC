/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "ns3/test.h"
#include "ns3/names.h"

using namespace ns3;

// ===========================================================================
// Cook up a couple of simple object class that we can use in the object 
// naming tests.  They do nothing but be of the right type.
// ===========================================================================
class TestObject : public Object
{
public:
  static TypeId GetTypeId (void) 
  {
    static TypeId tid = TypeId ("TestObject")
      .SetParent (Object::GetTypeId ())
      .HideFromDocumentation ()
      .AddConstructor<TestObject> ();
    return tid;
  }
  TestObject () {}
  virtual void Dispose (void) {}
};

class AlternateTestObject : public Object
{
public:
  static TypeId GetTypeId (void) 
  {
    static TypeId tid = TypeId ("AlternateTestObject")
      .SetParent (Object::GetTypeId ())
      .HideFromDocumentation ()
      .AddConstructor<AlternateTestObject> ();
    return tid;
  }
  AlternateTestObject () {}
  virtual void Dispose (void) {}
};

// ===========================================================================
// Test case to make sure that the Object Name Service can do its most basic
// job and add associations between Objects using the lowest level add 
// function, which is:
//
//   Add (Ptr<Object> context, std::string name, Ptr<Object> object);
//
// All other add functions will just translate into this form, so this is the
// most basic Add functionality.
// ===========================================================================
class BasicAddTestCase : public TestCase
{
public:
  BasicAddTestCase ();
  virtual ~BasicAddTestCase ();

private:
  virtual void DoRun (void);
  virtual void DoTeardown (void);
};

BasicAddTestCase::BasicAddTestCase ()
  : TestCase ("Check low level Names::Add and Names::FindName functionality")
{
}

BasicAddTestCase::~BasicAddTestCase ()
{
}

void
BasicAddTestCase::DoTeardown (void)
{
  Names::Clear ();
}

void
BasicAddTestCase::DoRun (void)
{
  std::string found;

  Ptr<TestObject> objectOne = CreateObject<TestObject> ();
  Names::Add (Ptr<Object> (0, false), "Name One", objectOne);

  Ptr<TestObject> objectTwo = CreateObject<TestObject> ();
  Names::Add (Ptr<Object> (0, false), "Name Two", objectTwo);

  Ptr<TestObject> childOfObjectOne = CreateObject<TestObject> ();
  Names::Add (objectOne, "Child", childOfObjectOne);

  Ptr<TestObject> childOfObjectTwo = CreateObject<TestObject> ();
  Names::Add (objectTwo, "Child", childOfObjectTwo);

  found = Names::FindName (objectOne);
  NS_TEST_ASSERT_MSG_EQ (found, "Name One", "Could not Names::Add and Names::FindName an Object");

  found = Names::FindName (objectTwo);
  NS_TEST_ASSERT_MSG_EQ (found, "Name Two", "Could not Names::Add and Names::FindName a second Object");

  found = Names::FindName (childOfObjectOne);
  NS_TEST_ASSERT_MSG_EQ (found, "Child", "Could not Names::Add and Names::FindName a child Object");

  found = Names::FindName (childOfObjectTwo);
  NS_TEST_ASSERT_MSG_EQ (found, "Child", "Could not Names::Add and Names::FindName a child Object");
}

// ===========================================================================
// Test case to make sure that the Object Name Service can correctly use a
// string context in the most basic ways
//
//   Add (std::string context, std::string name, Ptr<Object> object);
//
// High level path-based functions will translate into this form, so this is 
// the second most basic Add functionality.
// ===========================================================================
class StringContextAddTestCase : public TestCase
{
public:
  StringContextAddTestCase ();
  virtual ~StringContextAddTestCase ();

private:
  virtual void DoRun (void);
  virtual void DoTeardown (void);
};

StringContextAddTestCase::StringContextAddTestCase ()
  : TestCase ("Check string context Names::Add and Names::FindName functionality")

{
}

StringContextAddTestCase::~StringContextAddTestCase ()
{
}

void
StringContextAddTestCase::DoTeardown (void)
{
  Names::Clear ();
}

void
StringContextAddTestCase::DoRun (void)
{
  std::string found;

  Ptr<TestObject> objectOne = CreateObject<TestObject> ();
  Names::Add ("/Names", "Name One", objectOne);

  Ptr<TestObject> objectTwo = CreateObject<TestObject> ();
  Names::Add ("/Names", "Name Two", objectTwo);

  Ptr<TestObject> childOfObjectOne = CreateObject<TestObject> ();
  Names::Add ("/Names/Name One", "Child", childOfObjectOne);

  Ptr<TestObject> childOfObjectTwo = CreateObject<TestObject> ();
  Names::Add ("/Names/Name Two", "Child", childOfObjectTwo);

  found = Names::FindName (objectOne);
  NS_TEST_ASSERT_MSG_EQ (found, "Name One", "Could not Names::Add and Names::FindName an Object");

  found = Names::FindName (objectTwo);
  NS_TEST_ASSERT_MSG_EQ (found, "Name Two", "Could not Names::Add and Names::FindName a second Object");

  found = Names::FindName (childOfObjectOne);
  NS_TEST_ASSERT_MSG_EQ (found, "Child", "Could not Names::Add and Names::FindName a child Object");

  found = Names::FindName (childOfObjectTwo);
  NS_TEST_ASSERT_MSG_EQ (found, "Child", "Could not Names::Add and Names::FindName a child Object");
}

// ===========================================================================
// Test case to make sure that the Object Name Service can correctly use a
// fully qualified path to add assocations
//
//   Add (std::string name, Ptr<Object> object);
// ===========================================================================
class FullyQualifiedAddTestCase : public TestCase
{
public:
  FullyQualifiedAddTestCase ();
  virtual ~FullyQualifiedAddTestCase ();

private:
  virtual void DoRun (void);
  virtual void DoTeardown (void);
};

FullyQualifiedAddTestCase::FullyQualifiedAddTestCase ()
  : TestCase ("Check fully qualified path Names::Add and Names::FindName functionality")

{
}

FullyQualifiedAddTestCase::~FullyQualifiedAddTestCase ()
{
}

void
FullyQualifiedAddTestCase::DoTeardown (void)
{
  Names::Clear ();
}

void
FullyQualifiedAddTestCase::DoRun (void)
{
  std::string found;

  Ptr<TestObject> objectOne = CreateObject<TestObject> ();
  Names::Add ("/Names/Name One", objectOne);

  Ptr<TestObject> objectTwo = CreateObject<TestObject> ();
  Names::Add ("/Names/Name Two", objectTwo);

  Ptr<TestObject> childOfObjectOne = CreateObject<TestObject> ();
  Names::Add ("/Names/Name One/Child", childOfObjectOne);

  Ptr<TestObject> childOfObjectTwo = CreateObject<TestObject> ();
  Names::Add ("/Names/Name Two/Child", childOfObjectTwo);

  found = Names::FindName (objectOne);
  NS_TEST_ASSERT_MSG_EQ (found, "Name One", "Could not Names::Add and Names::FindName an Object");

  found = Names::FindName (objectTwo);
  NS_TEST_ASSERT_MSG_EQ (found, "Name Two", "Could not Names::Add and Names::FindName a second Object");

  found = Names::FindName (childOfObjectOne);
  NS_TEST_ASSERT_MSG_EQ (found, "Child", "Could not Names::Add and Names::FindName a child Object");

  found = Names::FindName (childOfObjectTwo);
  NS_TEST_ASSERT_MSG_EQ (found, "Child", "Could not Names::Add and Names::FindName a child Object");
}

// ===========================================================================
// Test case to make sure that the Object Name Service can correctly use a
// relative path to add assocations.  This functionality is provided as a 
// convenience so clients don't always have to provide the name service 
// namespace name in all of their strings.
//
//
//   Add (std::string name, Ptr<Object> object);
// ===========================================================================
class RelativeAddTestCase : public TestCase
{
public:
  RelativeAddTestCase ();
  virtual ~RelativeAddTestCase ();

private:
  virtual void DoRun (void);
  virtual void DoTeardown (void);
};

RelativeAddTestCase::RelativeAddTestCase ()
  : TestCase ("Check relative path Names::Add and Names::FindName functionality")

{
}

RelativeAddTestCase::~RelativeAddTestCase ()
{
}

void
RelativeAddTestCase::DoTeardown (void)
{
  Names::Clear ();
}

void
RelativeAddTestCase::DoRun (void)
{
  std::string found;

  Ptr<TestObject> objectOne = CreateObject<TestObject> ();
  Names::Add ("Name One", objectOne);

  Ptr<TestObject> objectTwo = CreateObject<TestObject> ();
  Names::Add ("Name Two", objectTwo);

  Ptr<TestObject> childOfObjectOne = CreateObject<TestObject> ();
  Names::Add ("Name One/Child", childOfObjectOne);

  Ptr<TestObject> childOfObjectTwo = CreateObject<TestObject> ();
  Names::Add ("Name Two/Child", childOfObjectTwo);

  found = Names::FindName (objectOne);
  NS_TEST_ASSERT_MSG_EQ (found, "Name One", "Could not Names::Add and Names::FindName an Object");

  found = Names::FindName (objectTwo);
  NS_TEST_ASSERT_MSG_EQ (found, "Name Two", "Could not Names::Add and Names::FindName a second Object");

  found = Names::FindName (childOfObjectOne);
  NS_TEST_ASSERT_MSG_EQ (found, "Child", "Could not Names::Add and Names::FindName a child Object");

  found = Names::FindName (childOfObjectTwo);
  NS_TEST_ASSERT_MSG_EQ (found, "Child", "Could not Names::Add and Names::FindName a child Object");
}

// ===========================================================================
// Test case to make sure that the Object Name Service can rename objects in
// its most basic way, which is 
//
//   Rename (Ptr<Object> context, std::string oldname, std::string newname);
//
// All other rename functions will just translate into this form, so this is the
// most basic rename functionality.
// ===========================================================================
class BasicRenameTestCase : public TestCase
{
public:
  BasicRenameTestCase ();
  virtual ~BasicRenameTestCase ();

private:
  virtual void DoRun (void);
  virtual void DoTeardown (void);
};

BasicRenameTestCase::BasicRenameTestCase ()
  : TestCase ("Check low level Names::Rename functionality")
{
}

BasicRenameTestCase::~BasicRenameTestCase ()
{
}

void
BasicRenameTestCase::DoTeardown (void)
{
  Names::Clear ();
}

void
BasicRenameTestCase::DoRun (void)
{
  std::string found;

  Ptr<TestObject> objectOne = CreateObject<TestObject> ();
  Names::Add (Ptr<Object> (0, false), "Name", objectOne);

  Ptr<TestObject> childOfObjectOne = CreateObject<TestObject> ();
  Names::Add (objectOne, "Child", childOfObjectOne);

  found = Names::FindName (objectOne);
  NS_TEST_ASSERT_MSG_EQ (found, "Name", "Could not Names::Add and Names::FindName an Object");

  Names::Rename (Ptr<Object> (0, false), "Name", "New Name");

  found = Names::FindName (objectOne);
  NS_TEST_ASSERT_MSG_EQ (found, "New Name", "Could not Names::Rename an Object");

  found = Names::FindName (childOfObjectOne);
  NS_TEST_ASSERT_MSG_EQ (found, "Child", "Could not Names::Add and Names::FindName a child Object");

  Names::Rename (objectOne, "Child", "New Child");

  found = Names::FindName (childOfObjectOne);
  NS_TEST_ASSERT_MSG_EQ (found, "New Child", "Could not Names::Rename a child Object");
}

// ===========================================================================
// Test case to make sure that the Object Name Service can rename objects 
// using a string context
//
//   Rename (std::string context, std::string oldname, std::string newname);
// ===========================================================================
class StringContextRenameTestCase : public TestCase
{
public:
  StringContextRenameTestCase ();
  virtual ~StringContextRenameTestCase ();

private:
  virtual void DoRun (void);
  virtual void DoTeardown (void);
};

StringContextRenameTestCase::StringContextRenameTestCase ()
  : TestCase ("Check string context-based Names::Rename functionality")
{
}

StringContextRenameTestCase::~StringContextRenameTestCase ()
{
}

void
StringContextRenameTestCase::DoTeardown (void)
{
  Names::Clear ();
}

void
StringContextRenameTestCase::DoRun (void)
{
  std::string found;

  Ptr<TestObject> objectOne = CreateObject<TestObject> ();
  Names::Add ("/Names", "Name", objectOne);

  Ptr<TestObject> childOfObjectOne = CreateObject<TestObject> ();
  Names::Add ("/Names/Name", "Child", childOfObjectOne);

  found = Names::FindName (objectOne);
  NS_TEST_ASSERT_MSG_EQ (found, "Name", "Could not Names::Add and Names::FindName an Object");

  Names::Rename ("/Names", "Name", "New Name");

  found = Names::FindName (objectOne);
  NS_TEST_ASSERT_MSG_EQ (found, "New Name", "Could not Names::Rename an Object");

  found = Names::FindName (childOfObjectOne);
  NS_TEST_ASSERT_MSG_EQ (found, "Child", "Could not Names::Add and Names::FindName a child Object");

  Names::Rename ("/Names/New Name", "Child", "New Child");

  found = Names::FindName (childOfObjectOne);
  NS_TEST_ASSERT_MSG_EQ (found, "New Child", "Could not Names::Rename a child Object");
}

// ===========================================================================
// Test case to make sure that the Object Name Service can rename objects 
// using a fully qualified path name
//
//   Rename (std::string oldpath, std::string newname);
// ===========================================================================
class FullyQualifiedRenameTestCase : public TestCase
{
public:
  FullyQualifiedRenameTestCase ();
  virtual ~FullyQualifiedRenameTestCase ();

private:
  virtual void DoRun (void);
  virtual void DoTeardown (void);
};

FullyQualifiedRenameTestCase::FullyQualifiedRenameTestCase ()
  : TestCase ("Check fully qualified path Names::Rename functionality")
{
}

FullyQualifiedRenameTestCase::~FullyQualifiedRenameTestCase ()
{
}

void
FullyQualifiedRenameTestCase::DoTeardown (void)
{
  Names::Clear ();
}

void
FullyQualifiedRenameTestCase::DoRun (void)
{
  std::string found;

  Ptr<TestObject> objectOne = CreateObject<TestObject> ();
  Names::Add ("/Names/Name", objectOne);

  Ptr<TestObject> childOfObjectOne = CreateObject<TestObject> ();
  Names::Add ("/Names/Name/Child", childOfObjectOne);

  found = Names::FindName (objectOne);
  NS_TEST_ASSERT_MSG_EQ (found, "Name", "Could not Names::Add and Names::FindName an Object");

  Names::Rename ("/Names/Name", "New Name");

  found = Names::FindName (objectOne);
  NS_TEST_ASSERT_MSG_EQ (found, "New Name", "Could not Names::Rename an Object");

  found = Names::FindName (childOfObjectOne);
  NS_TEST_ASSERT_MSG_EQ (found, "Child", "Could not Names::Add and Names::FindName a child Object");

  Names::Rename ("/Names/New Name/Child", "New Child");

  found = Names::FindName (childOfObjectOne);
  NS_TEST_ASSERT_MSG_EQ (found, "New Child", "Could not Names::Rename a child Object");
}

// ===========================================================================
// Test case to make sure that the Object Name Service can rename objects 
// using a relaltive path name
//
//   Rename (std::string oldpath, std::string newname);
// ===========================================================================
class RelativeRenameTestCase : public TestCase
{
public:
  RelativeRenameTestCase ();
  virtual ~RelativeRenameTestCase ();

private:
  virtual void DoRun (void);
  virtual void DoTeardown (void);
};

RelativeRenameTestCase::RelativeRenameTestCase ()
  : TestCase ("Check relative path Names::Rename functionality")
{
}

RelativeRenameTestCase::~RelativeRenameTestCase ()
{
}

void
RelativeRenameTestCase::DoTeardown (void)
{
  Names::Clear ();
}

void
RelativeRenameTestCase::DoRun (void)
{
  std::string found;

  Ptr<TestObject> objectOne = CreateObject<TestObject> ();
  Names::Add ("Name", objectOne);

  Ptr<TestObject> childOfObjectOne = CreateObject<TestObject> ();
  Names::Add ("Name/Child", childOfObjectOne);

  found = Names::FindName (objectOne);
  NS_TEST_ASSERT_MSG_EQ (found, "Name", "Could not Names::Add and Names::FindName an Object");

  Names::Rename ("Name", "New Name");

  found = Names::FindName (objectOne);
  NS_TEST_ASSERT_MSG_EQ (found, "New Name", "Could not Names::Rename an Object");

  found = Names::FindName (childOfObjectOne);
  NS_TEST_ASSERT_MSG_EQ (found, "Child", "Could not Names::Add and Names::FindName a child Object");

  Names::Rename ("New Name/Child", "New Child");

  found = Names::FindName (childOfObjectOne);
  NS_TEST_ASSERT_MSG_EQ (found, "New Child", "Could not Names::Rename a child Object");
}

// ===========================================================================
// Test case to make sure that the Object Name Service can look up an object
// and return its fully qualified path name
//
//   FindPath (Ptr<Object> object);
// ===========================================================================
class FindPathTestCase : public TestCase
{
public:
  FindPathTestCase ();
  virtual ~FindPathTestCase ();

private:
  virtual void DoRun (void);
  virtual void DoTeardown (void);
};

FindPathTestCase::FindPathTestCase ()
  : TestCase ("Check Names::FindPath functionality")
{
}

FindPathTestCase::~FindPathTestCase ()
{
}

void
FindPathTestCase::DoTeardown (void)
{
  Names::Clear ();
}

void
FindPathTestCase::DoRun (void)
{
  std::string found;

  Ptr<TestObject> objectOne = CreateObject<TestObject> ();
  Names::Add ("Name", objectOne);

  Ptr<TestObject> childOfObjectOne = CreateObject<TestObject> ();
  Names::Add ("/Names/Name/Child", childOfObjectOne);

  found = Names::FindPath (objectOne);
  NS_TEST_ASSERT_MSG_EQ (found, "/Names/Name", "Could not Names::Add and Names::FindPath an Object");

  found = Names::FindPath (childOfObjectOne);
  NS_TEST_ASSERT_MSG_EQ (found, "/Names/Name/Child", "Could not Names::Add and Names::FindPath a child Object");

  Ptr<TestObject> objectNotThere = CreateObject<TestObject> ();
  found = Names::FindPath (objectNotThere);
  NS_TEST_ASSERT_MSG_EQ (found, "", "Unexpectedly found a non-existent Object");
}

// ===========================================================================
// Test case to make sure that the Object Name Service can find Objects using 
// the lowest level find function, which is:
//
//   Find (Ptr<Object> context, std::string name);
// ===========================================================================
class BasicFindTestCase : public TestCase
{
public:
  BasicFindTestCase ();
  virtual ~BasicFindTestCase ();

private:
  virtual void DoRun (void);
  virtual void DoTeardown (void);
};

BasicFindTestCase::BasicFindTestCase ()
  : TestCase ("Check low level Names::Find functionality")
{
}

BasicFindTestCase::~BasicFindTestCase ()
{
}

void
BasicFindTestCase::DoTeardown (void)
{
  Names::Clear ();
}

void
BasicFindTestCase::DoRun (void)
{
  Ptr<TestObject> found;

  Ptr<TestObject> objectOne = CreateObject<TestObject> ();
  Names::Add ("Name One", objectOne);

  Ptr<TestObject> objectTwo = CreateObject<TestObject> ();
  Names::Add ("Name Two", objectTwo);

  Ptr<TestObject> childOfObjectOne = CreateObject<TestObject> ();
  Names::Add ("Name One/Child", childOfObjectOne);

  Ptr<TestObject> childOfObjectTwo = CreateObject<TestObject> ();
  Names::Add ("Name Two/Child", childOfObjectTwo);

  found = Names::Find<TestObject> (Ptr<Object> (0, false), "Name One");
  NS_TEST_ASSERT_MSG_EQ (found, objectOne, "Could not find a previously named Object via object context");

  found = Names::Find<TestObject> (Ptr<Object> (0, false), "Name Two");
  NS_TEST_ASSERT_MSG_EQ (found, objectTwo, "Could not find a previously named Object via object context");

  found = Names::Find<TestObject> (objectOne, "Child");
  NS_TEST_ASSERT_MSG_EQ (found, childOfObjectOne, "Could not find a previously named child Object via object context");

  found = Names::Find<TestObject> (objectTwo, "Child");
  NS_TEST_ASSERT_MSG_EQ (found, childOfObjectTwo, "Could not find a previously named child Object via object context");
}

// ===========================================================================
// Test case to make sure that the Object Name Service can find Objects using 
// a string context-based find function, which is:
//
//   Find (std::string context, std::string name);
// ===========================================================================
class StringContextFindTestCase : public TestCase
{
public:
  StringContextFindTestCase ();
  virtual ~StringContextFindTestCase ();

private:
  virtual void DoRun (void);
  virtual void DoTeardown (void);
};

StringContextFindTestCase::StringContextFindTestCase ()
  : TestCase ("Check string context-based Names::Find functionality")
{
}

StringContextFindTestCase::~StringContextFindTestCase ()
{
}

void
StringContextFindTestCase::DoTeardown (void)
{
  Names::Clear ();
}

void
StringContextFindTestCase::DoRun (void)
{
  Ptr<TestObject> found;

  Ptr<TestObject> objectOne = CreateObject<TestObject> ();
  Names::Add ("Name One", objectOne);

  Ptr<TestObject> objectTwo = CreateObject<TestObject> ();
  Names::Add ("Name Two", objectTwo);

  Ptr<TestObject> childOfObjectOne = CreateObject<TestObject> ();
  Names::Add ("Name One/Child", childOfObjectOne);

  Ptr<TestObject> childOfObjectTwo = CreateObject<TestObject> ();
  Names::Add ("Name Two/Child", childOfObjectTwo);

  found = Names::Find<TestObject> ("/Names", "Name One");
  NS_TEST_ASSERT_MSG_EQ (found, objectOne, "Could not find a previously named Object via string context");

  found = Names::Find<TestObject> ("/Names", "Name Two");
  NS_TEST_ASSERT_MSG_EQ (found, objectTwo, "Could not find a previously named Object via stribng context");

  found = Names::Find<TestObject> ("/Names/Name One", "Child");
  NS_TEST_ASSERT_MSG_EQ (found, childOfObjectOne, "Could not find a previously named child Object via string context");

  found = Names::Find<TestObject> ("/Names/Name Two", "Child");
  NS_TEST_ASSERT_MSG_EQ (found, childOfObjectTwo, "Could not find a previously named child Object via string context");
}

// ===========================================================================
// Test case to make sure that the Object Name Service can find Objects using 
// a fully qualified path name-based find function, which is:
//
//   Find (std::string name);
// ===========================================================================
class FullyQualifiedFindTestCase : public TestCase
{
public:
  FullyQualifiedFindTestCase ();
  virtual ~FullyQualifiedFindTestCase ();

private:
  virtual void DoRun (void);
  virtual void DoTeardown (void);
};

FullyQualifiedFindTestCase::FullyQualifiedFindTestCase ()
  : TestCase ("Check fully qualified path Names::Find functionality")
{
}

FullyQualifiedFindTestCase::~FullyQualifiedFindTestCase ()
{
}

void
FullyQualifiedFindTestCase::DoTeardown (void)
{
  Names::Clear ();
}

void
FullyQualifiedFindTestCase::DoRun (void)
{
  Ptr<TestObject> found;

  Ptr<TestObject> objectOne = CreateObject<TestObject> ();
  Names::Add ("/Names/Name One", objectOne);

  Ptr<TestObject> objectTwo = CreateObject<TestObject> ();
  Names::Add ("/Names/Name Two", objectTwo);

  Ptr<TestObject> childOfObjectOne = CreateObject<TestObject> ();
  Names::Add ("/Names/Name One/Child", childOfObjectOne);

  Ptr<TestObject> childOfObjectTwo = CreateObject<TestObject> ();
  Names::Add ("/Names/Name Two/Child", childOfObjectTwo);

  found = Names::Find<TestObject> ("/Names/Name One");
  NS_TEST_ASSERT_MSG_EQ (found, objectOne, "Could not find a previously named Object via string context");

  found = Names::Find<TestObject> ("/Names/Name Two");
  NS_TEST_ASSERT_MSG_EQ (found, objectTwo, "Could not find a previously named Object via stribng context");

  found = Names::Find<TestObject> ("/Names/Name One/Child");
  NS_TEST_ASSERT_MSG_EQ (found, childOfObjectOne, "Could not find a previously named child Object via string context");

  found = Names::Find<TestObject> ("/Names/Name Two/Child");
  NS_TEST_ASSERT_MSG_EQ (found, childOfObjectTwo, "Could not find a previously named child Object via string context");
}

// ===========================================================================
// Test case to make sure that the Object Name Service can find Objects using 
// a relative path name-based find function, which is:
//
//   Find (std::string name);
// ===========================================================================
class RelativeFindTestCase : public TestCase
{
public:
  RelativeFindTestCase ();
  virtual ~RelativeFindTestCase ();

private:
  virtual void DoRun (void);
  virtual void DoTeardown (void);
};

RelativeFindTestCase::RelativeFindTestCase ()
  : TestCase ("Check relative path Names::Find functionality")
{
}

RelativeFindTestCase::~RelativeFindTestCase ()
{
}

void
RelativeFindTestCase::DoTeardown (void)
{
  Names::Clear ();
}

void
RelativeFindTestCase::DoRun (void)
{
  Ptr<TestObject> found;

  Ptr<TestObject> objectOne = CreateObject<TestObject> ();
  Names::Add ("Name One", objectOne);

  Ptr<TestObject> objectTwo = CreateObject<TestObject> ();
  Names::Add ("Name Two", objectTwo);

  Ptr<TestObject> childOfObjectOne = CreateObject<TestObject> ();
  Names::Add ("Name One/Child", childOfObjectOne);

  Ptr<TestObject> childOfObjectTwo = CreateObject<TestObject> ();
  Names::Add ("Name Two/Child", childOfObjectTwo);

  found = Names::Find<TestObject> ("Name One");
  NS_TEST_ASSERT_MSG_EQ (found, objectOne, "Could not find a previously named Object via string context");

  found = Names::Find<TestObject> ("Name Two");
  NS_TEST_ASSERT_MSG_EQ (found, objectTwo, "Could not find a previously named Object via stribng context");

  found = Names::Find<TestObject> ("Name One/Child");
  NS_TEST_ASSERT_MSG_EQ (found, childOfObjectOne, "Could not find a previously named child Object via string context");

  found = Names::Find<TestObject> ("Name Two/Child");
  NS_TEST_ASSERT_MSG_EQ (found, childOfObjectTwo, "Could not find a previously named child Object via string context");
}

// ===========================================================================
// Test case to make sure that the Object Name Service can find Objects using 
// a second type.
// ===========================================================================
class AlternateFindTestCase : public TestCase
{
public:
  AlternateFindTestCase ();
  virtual ~AlternateFindTestCase ();

private:
  virtual void DoRun (void);
  virtual void DoTeardown (void);
};

AlternateFindTestCase::AlternateFindTestCase ()
  : TestCase ("Check GetObject operation in Names::Find")
{
}

AlternateFindTestCase::~AlternateFindTestCase ()
{
}

void
AlternateFindTestCase::DoTeardown (void)
{
  Names::Clear ();
}

void
AlternateFindTestCase::DoRun (void)
{
  Ptr<TestObject> testObject = CreateObject<TestObject> ();
  Names::Add ("Test Object", testObject);

  Ptr<AlternateTestObject> alternateTestObject = CreateObject<AlternateTestObject> ();
  Names::Add ("Alternate Test Object", alternateTestObject);

  Ptr<TestObject> foundTestObject;
  Ptr<AlternateTestObject> foundAlternateTestObject;

  foundTestObject = Names::Find<TestObject> ("Test Object");
  NS_TEST_ASSERT_MSG_EQ (foundTestObject, testObject, 
                         "Could not find a previously named TestObject via GetObject");

  foundAlternateTestObject = Names::Find<AlternateTestObject> ("Alternate Test Object");
  NS_TEST_ASSERT_MSG_EQ (foundAlternateTestObject, alternateTestObject, 
                         "Could not find a previously named AlternateTestObject via GetObject");


  foundAlternateTestObject = Names::Find<AlternateTestObject> ("Test Object");
  NS_TEST_ASSERT_MSG_EQ (foundAlternateTestObject, 0, 
                         "Unexpectedly able to GetObject<AlternateTestObject> on a TestObject");

  foundTestObject = Names::Find<TestObject> ("Alternate Test Object");
  NS_TEST_ASSERT_MSG_EQ (foundTestObject, 0, 
                         "Unexpectedly able to GetObject<TestObject> on an AlternateTestObject");
}

class NamesTestSuite : public TestSuite
{
public:
  NamesTestSuite ();
};

NamesTestSuite::NamesTestSuite ()
  : TestSuite ("object-name-service", UNIT)
{
  AddTestCase (new BasicAddTestCase);
  AddTestCase (new StringContextAddTestCase);
  AddTestCase (new FullyQualifiedAddTestCase);
  AddTestCase (new RelativeAddTestCase);
  AddTestCase (new BasicRenameTestCase);
  AddTestCase (new StringContextRenameTestCase);
  AddTestCase (new FullyQualifiedRenameTestCase);
  AddTestCase (new RelativeRenameTestCase);
  AddTestCase (new FindPathTestCase);
  AddTestCase (new BasicFindTestCase);
  AddTestCase (new StringContextFindTestCase);
  AddTestCase (new FullyQualifiedFindTestCase);
  AddTestCase (new RelativeFindTestCase);
  AddTestCase (new AlternateFindTestCase);
}

static NamesTestSuite namesTestSuite;
