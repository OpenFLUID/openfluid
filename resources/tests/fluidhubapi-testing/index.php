<?php
#
#   This file is part of OpenFLUID software
#   Copyright(c) 2007, INRA - Montpellier SupAgro
#
#
#  == GNU General Public License Usage ==
#
#   OpenFLUID is free software: you can redistribute it and/or modify
#   it under the terms of the GNU General Public License as published by
#   the Free Software Foundation, either version 3 of the License, or
#   (at your option) any later version.
#
#   OpenFLUID is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#   GNU General Public License for more details.
#
#   You should have received a copy of the GNU General Public License
#   along with OpenFLUID. If not, see <http://www.gnu.org/licenses/>.
#
#
#  == Other Usage ==
#
#   Other Usage means a use of OpenFLUID that is inconsistent with the GPL
#   license, and requires a written agreement between You and INRA.
#   Licensees for Other Usage of OpenFLUID may use this file in accordance
#   with the terms contained in the written agreement between You and INRA.
#


error_reporting(0);


require("Slim/Slim.php");


\Slim\Slim::registerAutoloader();

$app = new \Slim\Slim();


// =====================================================================
// =====================================================================


$app->get('/',function () use($app)
{
  header( "Content-Type: application/json; charset=UTF8" );
  echo "{\n";
  echo "  \"nature\" : \"OpenFLUID FluidHub\",\n";
  echo "  \"api-version\" : \"1.0.20150107\",\n";
  echo "  \"capabilities\" : [\"news\",\"wareshub\"],\n";
  echo "  \"status\" : \"testing\",\n";
  echo "  \"name\" : \"fluidhub for testing\"\n";
  echo "}";
});


// =====================================================================
// =====================================================================


$app->get('/news',function () use($app)
{
  $lang = $app->request()->params('lang');

  if (empty($lang)) 
    $lang = "en";

  $RSSContent = "";
  
  header("Content-Type: application/rss+xml; charset=UTF8");

  if ($lang == "en")
  {
    $RSSContent = <<<EOD
<rss version="2.0">
  <channel>
    <title></title>
    <description></description>
    <link></link>
    <copyright>Copyrigh INRA-SupAgro</copyright>

    <item>
      <title>Item 1</title>
      <description>This is Item 1</description>
      <link>http://item1.org</link>
      <pubDate>2105-09-30</pubDate>
      <category>doc</category>
    </item>

    <item>
      <title>Item 2</title>
      <description>This is Item 2</description>
      <link>http://item2.org</link>
      <pubDate>2105-09-26</pubDate>
      <category>software</category>
    </item>

    <item>
      <title>Item 3</title>
      <description>This is Item 3</description>
      <link>http://item3.org</link>
      <pubDate>2105-09-12</pubDate>
      <category>training</category>
    </item>

  </channel>
</rss>
EOD;
  }
  else if ($lang == "fr")
  {
$RSSContent = <<<EOD
<rss version="2.0">
  <channel>
    <title></title>
    <description></description>
    <link></link>
    <copyright>Droits réservés INRA-SupAgro</copyright>

    <item>
      <title>Article 1</title>
      <description>C'est l'élément 1</description>
      <link>http://item1.org</link>
      <pubDate>2105-09-30</pubDate>
      <category>doc</category>
    </item>

  </channel>
</rss>
EOD;
  }
  
  echo $RSSContent;
});


// =====================================================================
// =====================================================================


$app->get('/wares',function () use($app)
{
  $detailed = $app->request()->params('detailed');
  $username = $app->request()->params('username');

  header( "Content-Type: application/json; charset=UTF8" );

  $JSONContent = "";


  if ($detailed == "yes")
  {
    if ($username != "")
      $username .= "@";

    $JSONContent = <<<EOD
{
  "simulators" : {
    "examples.sim.A" : {
      "shortdesc" : "This is examples.sim.A...",
      "git-url" : "https://{$username}host.domain.org/foo-wareshub/git/simulators/examples.sim.A",
      "users-ro" : ["*"],
      "users-rw" : ["user1","user2","user3"]
    },
    "examples.sim.B" : {
      "shortdesc" : "This is examples.sim.B...",
      "git-url" : "https://{$username}host.domain.org/foo-wareshub/git/simulators/examples.sim.B",
      "users-ro" : ["*"],
      "users-rw" : ["user1"]
    },
    "examples.sim.C" : {
      "shortdesc" : "This is examples.sim.C...",
      "git-url" : "https://{$username}host.domain.org/foo-wareshub/git/simulators/examples.sim.C",
      "users-ro" : ["*"],
      "users-rw" : ["user2","user3"]
    },
    "examples.sim.D" : {
      "shortdesc" : "This is examples.sim.D...",
      "git-url" : "https://{$username}host.domain.org/foo-wareshub/git/simulators/examples.sim.D",
      "users-ro" : ["*"],
      "users-rw" : ["user1"]
    }
  },
  "observers" : {
    "examples.obs.J" : {
      "shortdesc" : "This is examples.obs.J...",
      "git-url" : "https://{$username}host.domain.org/foo-wareshub/git/observers/examples.obs.J",
      "users-ro" : ["user1"],
      "users-rw" : ["user2"]
    },
    "examples.obs.K" : {
      "shortdesc" : "This is examples.obs.K...",
      "git-url" : "https://{$username}host.domain.org/foo-wareshub/git/observers/examples.obs.K",
      "users-ro" : ["*"],
      "users-rw" : ["user1"]
    }
  },
  "builderexts" : {
    "examples.bext.X" : {
      "shortdesc" : "This is examples.bext.X...",
      "git-url" : "https://{$username}host.domain.org/foo-wareshub/git/builderexts/examples.bext.X",
      "users-ro" : [],
      "users-rw" : ["*"]
    }
  }
}
EOD;

  }
  else
  {
    $JSONContent = <<<EOD
{
  "simulators" : [
    "examples.sim.A",
    "examples.sim.B",
    "examples.sim.C",
    "examples.sim.D"
  ],
  "observers" : [
    "examples.obs.J",
    "examples.obs.K"
  ],
  "builderexts" : [
    "examples.bext.X"
  ]
}
EOD;
  }

  echo $JSONContent;
});


// =====================================================================
// =====================================================================


$app->get('/wares/simulators',function () use($app)
{
  $detailed = $app->request()->params('detailed');
  $username = $app->request()->params('username');

  header( "Content-Type: application/json; charset=UTF8" );

  $JSONContent = "";


  if ($detailed == "yes")
  {
    if ($username != "")
      $username .= "@";

    $JSONContent = <<<EOD
{
    "simulators" : {
      "examples.sim.A" : {
        "shortdesc" : "This is examples.sim.A...",
        "git-url" : "https://{$username}host.domain.org/foo-wareshub/git/simulators/examples.sim.A",
        "users-ro" : ["*"],
        "users-rw" : ["user1","user2","user3"]
      },
      "examples.sim.B" : {
        "shortdesc" : "This is examples.sim.B...",
        "git-url" : "https://{$username}host.domain.org/foo-wareshub/git/simulators/examples.sim.B",
        "users-ro" : ["*"],
        "users-rw" : ["user1"]
      },
      "examples.sim.C" : {
        "shortdesc" : "This is examples.sim.C...",
        "git-url" : "https://{$username}host.domain.org/foo-wareshub/git/simulators/examples.sim.C",
        "users-ro" : ["*"],
        "users-rw" : ["user2","user3"]
      },
      "examples.sim.D" : {
        "shortdesc" : "This is examples.sim.D...",
        "git-url" : "https://{$username}host.domain.org/foo-wareshub/git/simulators/examples.sim.D",
        "users-ro" : ["*"],
        "users-rw" : ["user1"]
      }
    }
}
EOD;

  }
  else
  {
    $JSONContent = <<<EOD
{
  "simulators" : [
    "examples.sim.A",
    "examples.sim.B",
    "examples.sim.C",
    "examples.sim.D"
  ]
}
EOD;
  }

  echo $JSONContent;

});


// =====================================================================
// =====================================================================


$app->get('/wares/observers',function () use($app)
{
  $detailed = $app->request()->params('detailed');
  $username = $app->request()->params('username');

  header( "Content-Type: application/json; charset=UTF8" );

  $JSONContent = "";


  if ($detailed == "yes")
  {
    if ($username != "")
      $username .= "@";

    $JSONContent = <<<EOD
{
  "observers" : {
    "examples.obs.J" : {
      "shortdesc" : "This is examples.obs.J...",
      "git-url" : "https://{$username}host.domain.org/foo-wareshub/git/observers/examples.obs.J",
      "users-ro" : ["user1"],
      "users-rw" : ["user2"]
    },
    "examples.obs.K" : {
      "shortdesc" : "This is examples.obs.K...",
      "git-url" : "https://{$username}host.domain.org/foo-wareshub/git/observers/examples.obs.K",
      "users-ro" : ["*"],
      "users-rw" : ["user1"]
    }
  }
}
EOD;

  }
  else
  {
    $JSONContent = <<<EOD
{
  "observers" : [
    "examples.obs.J",
    "examples.obs.K"
  ]
}
EOD;
  }

  echo $JSONContent;

});


// =====================================================================
// =====================================================================


$app->get('/wares/builderexts',function () use($app)
{
  $detailed = $app->request()->params('detailed');
  $username = $app->request()->params('username');

  header( "Content-Type: application/json; charset=UTF8" );

  $JSONContent = "";


  if ($detailed == "yes")
  {
    if ($username != "")
      $username .= "@";

    $JSONContent = <<<EOD
{
  "builderexts" : {
    "examples.bext.X" : {
      "shortdesc" : "This is examples.bext.X...",
      "git-url" : "https://{$username}host.domain.org/foo-wareshub/git/builderexts/examples.bext.X",
      "users-ro" : [],
      "users-rw" : ["*"]
    }
  }
}
EOD;

  }
  else
  {
    $JSONContent = <<<EOD
{
  "builderexts" : [
    "examples.bext.X"
  ]
}
EOD;
  }

  echo $JSONContent;

});


// =====================================================================
// =====================================================================


// handle errors
$app->notFound(function ()
{
  header('HTTP/1.1 400 Bad Request',true,400);
});


// =====================================================================
// =====================================================================


$app->run();

?>
