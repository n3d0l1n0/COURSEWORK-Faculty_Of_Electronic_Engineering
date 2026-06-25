using DS_LV1_PRVI;
using Grpc.Core;
using DS_LV1_PRVI;

namespace DS_LV1_PRVI.Services
{
    public class UserServiceImpl : UserService.UserServiceBase
    {
        private static readonly List<User> users = new();

        public override Task<CreateUserResponse> CreateUser(User request, ServerCallContext context)
        {
            users.Add(request);

            return Task.FromResult(
                new CreateUserResponse
                {
                    Message = "Korisnik dodat."
                });
        }

        public override Task<User> GetUser(UserIdRequest request, ServerCallContext context)
        {
            var user = users.FirstOrDefault(x => x.Id == request.Id);

            if (user == null)
                throw new RpcException(new Status(StatusCode.NotFound,"Korisnik nije pronadjen"));

            return Task.FromResult(user);
        }

        public override Task<UpdateUserResponse> UpdateUser(User request, ServerCallContext context)
        {
            var user = users.FirstOrDefault(x => x.Id == request.Id);

            if (user == null)
            {
                return Task.FromResult(new UpdateUserResponse
                    {
                        Message = "Korisnik nije pronadjen."
                    });
            }

            user.FirstName = request.FirstName;
            user.LastName = request.LastName;
            user.Address = request.Address;

            user.Phones.Clear();

            foreach (var p in request.Phones)
                user.Phones.Add(p);

            return Task.FromResult(
            new UpdateUserResponse
            {
                Message = "Korisnik apdejtovan"
            });
        }

        public override Task<DeleteUserResponse> DeleteUser(UserIdRequest request,ServerCallContext context)
        {
            var user = users.FirstOrDefault(x => x.Id == request.Id);

            if (user == null)
            {
                return Task.FromResult(
                    new DeleteUserResponse
                    {
                        Message = "Korisnik nije pronadjen"
                    });
            }

            users.Remove(user);

            return Task.FromResult(
                new DeleteUserResponse
                {
                    Message = "User deleted."
                });
        }

        public override Task<UserList> GetUsersInRange(RangeRequest request, ServerCallContext context)
        {
            var result = new UserList();

            result.Users.AddRange(
                users.Where(x =>
                    x.Id >= request.FromId &&
                    x.Id <= request.ToId));

            return Task.FromResult(result);
        }

        public override Task<DeleteMessages> DeleteUsers(DeleteIdsRequest request, ServerCallContext context)
        {
            var response = new DeleteMessages();

            foreach (var id in request.Ids)
            {
                var user = users.FirstOrDefault(x => x.Id == id);

                if (user != null)
                {
                    users.Remove(user);

                    response.Messages.Add(
                        $"Korisnik sa idem {id} obrisan.");
                }
                else
                {
                    response.Messages.Add(
                        $"Korisnik sa idem {id} nije pronadjen.");
                }
            }

            return Task.FromResult(response);
        }
    }
}