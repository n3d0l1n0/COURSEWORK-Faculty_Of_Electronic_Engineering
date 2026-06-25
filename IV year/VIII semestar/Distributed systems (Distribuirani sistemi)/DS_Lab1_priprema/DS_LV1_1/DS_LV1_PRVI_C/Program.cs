using DS_LV1_PRVI;
using Grpc.Net.Client;

var channel = GrpcChannel.ForAddress("http://localhost:5031");

var client = new UserService.UserServiceClient(channel);

var user = new User
{
    Id = 1,
    FirstName = "Neda",
    LastName = "Stojanovic",
    Address = "Nis"
};

user.Phones.Add(new PhoneNumber
{
    Number = "06752525"
});

user.Phones.Add(new PhoneNumber
{
    Number = "0627261515"
});

var create = await client.CreateUserAsync(user);
Console.WriteLine(create.Message);

var user2 = new User 
{
    Id = 2,
    FirstName = "Nena",
    LastName = "Stojanovic",
    Address = "Nis"
};
var create2 = await client.CreateUserAsync(user2); 
var fetched = await client.GetUserAsync(new UserIdRequest { Id = 1 });

Console.WriteLine($"{fetched.FirstName} {fetched.LastName}");

var range =await client.GetUsersInRangeAsync(
new RangeRequest
{
     FromId = 1,
     ToId = 2
});

Console.WriteLine("\nUsers in range:");

foreach (var u in range.Users)
{
    Console.WriteLine($"{u.Id} {u.FirstName} {u.LastName}");
}

var updatedUser = new User
{
    Id = 1,
    FirstName = "Neda",
    LastName = "Stojanovic Updated",
    Address = "Novi Sad"
};

updatedUser.Phones.Add(new PhoneNumber
{
    Number = "06351515151"
});

var updateResult = await client.UpdateUserAsync(updatedUser);
Console.WriteLine(updateResult.Message);

var afterUpdate = await client.GetUserAsync(new UserIdRequest { Id = 1 });
Console.WriteLine($"After update: {afterUpdate.FirstName} {afterUpdate.LastName}");

var deleteResponse = await client.DeleteUsersAsync(new DeleteIdsRequest
{
    Ids = { 1, 2, 3 }
});

Console.WriteLine("\nDelete results:");

foreach (var msg in deleteResponse.Messages)
{
    Console.WriteLine(msg);
}